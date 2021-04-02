/**
 * @file lv_fs_fatfs.c
 * For ESP32
 */

 /*********************
  *      INCLUDES
  *********************/
#include "lv_port_fatfs.h"


  /*********************
   *      DEFINES
   *********************/
#define DRIVE_LETTER 'S'
   /**********************
	*      TYPEDEFS
	**********************/

	/* Create a type to store the required data about your file.*/
typedef  FIL file_t;

/*Similarly to `file_t` create a type for directory reading too */
typedef  FF_DIR dir_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void fs_init(void);

static lv_fs_res_t fs_open(lv_fs_drv_t* drv, void* file_p, const char* path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close(lv_fs_drv_t* drv, void* file_p);
static lv_fs_res_t fs_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br);
static lv_fs_res_t fs_write(lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw);
static lv_fs_res_t fs_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos);
static lv_fs_res_t fs_size(lv_fs_drv_t* drv, void* file_p, uint32_t* size_p);
static lv_fs_res_t fs_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p);
static lv_fs_res_t fs_remove(lv_fs_drv_t* drv, const char* path);
static lv_fs_res_t fs_trunc(lv_fs_drv_t* drv, void* file_p);
static lv_fs_res_t fs_rename(lv_fs_drv_t* drv, const char* oldname, const char* newname);
static lv_fs_res_t fs_free(lv_fs_drv_t* drv, uint32_t* total_p, uint32_t* free_p);
static lv_fs_res_t fs_dir_open(lv_fs_drv_t* drv, void* dir_p, const char* path);
static lv_fs_res_t fs_dir_read(lv_fs_drv_t* drv, void* dir_p, char* fn);
static lv_fs_res_t fs_dir_close(lv_fs_drv_t* drv, void* dir_p);

/**********************
 *  STATIC VARIABLES
 **********************/

 /**********************
  *      MACROS
  **********************/

  /**********************
   *   GLOBAL FUNCTIONS
   **********************/

void lv_fs_if_init(void)
{
	/*----------------------------------------------------
	 * Initialize your storage device and File System
	 * -------------------------------------------------*/
	fs_init();

	/*---------------------------------------------------
	 * Register the file system interface  in LittlevGL
	 *--------------------------------------------------*/

	 /* Add a simple drive to open images */
	lv_fs_drv_t fs_drv;                         /*A driver descriptor*/
	lv_fs_drv_init(&fs_drv);

	/*Set up fields...*/
	fs_drv.file_size = sizeof(file_t);
	fs_drv.letter = DRIVE_LETTER;
	fs_drv.open_cb = fs_open;
	fs_drv.close_cb = fs_close;
	fs_drv.read_cb = fs_read;
	fs_drv.write_cb = fs_write;
	fs_drv.seek_cb = fs_seek;
	fs_drv.tell_cb = fs_tell;
	fs_drv.free_space_cb = fs_free;
	fs_drv.size_cb = fs_size;
	fs_drv.remove_cb = fs_remove;
	fs_drv.rename_cb = fs_rename;
	fs_drv.trunc_cb = fs_trunc;

	fs_drv.rddir_size = sizeof(dir_t);
	fs_drv.dir_close_cb = fs_dir_close;
	fs_drv.dir_open_cb = fs_dir_open;
	fs_drv.dir_read_cb = fs_dir_read;

	lv_fs_drv_register(&fs_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

 /* Initialize your Storage device and File system. */
static void fs_init(void)
{
	///* Initialisation de la carte SD */
	//Serial.print(F("Init SD card... "));

	//SPIClass* sd_spi = new SPIClass(HSPI); // another SPI
	//if (!SD.begin(15, *sd_spi)) // SD-Card SS pin is 15
	//{
	//	Serial.println("Card Mount Failed");
	//	return;
	//}
}

/**
 * Open a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param path path to the file beginning with the driver letter (e.g. S:/folder/file.txt)
 * @param mode read: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_open(lv_fs_drv_t* drv, void* file_p, const char* path, lv_fs_mode_t mode)
{
	uint8_t flags = 0;

	if (mode == LV_FS_MODE_WR) flags = FA_WRITE | FA_OPEN_ALWAYS;
	else if (mode == LV_FS_MODE_RD) flags = FA_READ;
	else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD)) flags = FA_READ | FA_WRITE | FA_OPEN_ALWAYS;

	FRESULT res = f_open((file_t*)file_p, path, flags);

	if (res == FR_OK)
	{
		f_lseek((file_t*)file_p, 0);
		return LV_FS_RES_OK;
	}
	else
	{
		return LV_FS_RES_UNKNOWN;
	}
}


/**
 * Close an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_close(lv_fs_drv_t* drv, void* file_p)
{
	f_close((file_t*)file_p);
	return LV_FS_RES_OK;
}

/**
 * Read data from an opened file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param buf pointer to a memory block where to store the read data
 * @param btr number of Bytes To Read
 * @param br the real number of read bytes (Byte Read)
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br)
{
	FRESULT res = f_read((file_t*)file_p, buf, btr, (UINT*)br);
	if (res == FR_OK) return LV_FS_RES_OK;
	else return LV_FS_RES_UNKNOWN;
}

/**
 * Write into a file
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param buf pointer to a buffer with the bytes to write
 * @param btr Bytes To Write
 * @param br the number of real written bytes (Bytes Written). NULL if unused.
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_write(lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw)
{
	FRESULT res = f_write((file_t*)file_p, buf, btw, (UINT*)bw);
	if (res == FR_OK) return LV_FS_RES_OK;
	else return LV_FS_RES_UNKNOWN;
}

/**
 * Set the read write pointer. Also expand the file size if necessary.
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable. (opened with lv_ufs_open )
 * @param pos the new position of read write pointer
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos)
{
	f_lseek((file_t*)file_p, pos);
	return LV_FS_RES_OK;
}

/**
 * Give the size of a file bytes
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable
 * @param size pointer to a variable to store the size
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_size(lv_fs_drv_t* drv, void* file_p, uint32_t* size_p)
{
	(*size_p) = f_size(((file_t*)file_p));
	return LV_FS_RES_OK;
}

/**
 * Give the position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to a file_t variable.
 * @param pos_p pointer to to store the result
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p)
{
	*pos_p = f_tell(((file_t*)file_p));
	return LV_FS_RES_OK;
}

/**
 * Delete a file
 * @param drv pointer to a driver where this function belongs
 * @param path path of the file to delete
 * @return  LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_remove(lv_fs_drv_t* drv, const char* path)
{
	lv_fs_res_t res = LV_FS_RES_NOT_IMP;

	/* Add your code here*/

	return res;
}

/**
 * Truncate the file size to the current position of the read write pointer
 * @param drv pointer to a driver where this function belongs
 * @param file_p pointer to an 'ufs_file_t' variable. (opened with lv_fs_open )
 * @return LV_FS_RES_OK: no error, the file is read
 *         any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_trunc(lv_fs_drv_t* drv, void* file_p)
{
	f_sync((file_t*)file_p);           /*If not syncronized fclose can write the truncated part*/
	f_truncate((file_t*)file_p);
	return LV_FS_RES_OK;
}

/**
 * Rename a file
 * @param drv pointer to a driver where this function belongs
 * @param oldname path to the file
 * @param newname path with the new name
 * @return LV_FS_RES_OK or any error from 'fs_res_t'
 */
static lv_fs_res_t fs_rename(lv_fs_drv_t* drv, const char* oldname, const char* newname)
{

	FRESULT res = f_rename(oldname, newname);

	if (res == FR_OK) return LV_FS_RES_OK;
	else return LV_FS_RES_UNKNOWN;
}

/**
 * Get the free and total size of a driver in kB
 * @param drv pointer to a driver where this function belongs
 * @param letter the driver letter
 * @param total_p pointer to store the total size [kB]
 * @param free_p pointer to store the free size [kB]
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_free(lv_fs_drv_t* drv, uint32_t* total_p, uint32_t* free_p)
{
	lv_fs_res_t res = LV_FS_RES_NOT_IMP;

	/* Add your code here*/

	return res;
}

/**
 * Initialize a 'fs_read_dir_t' variable for directory reading
 * @param drv pointer to a driver where this function belongs
 * @param dir_p pointer to a 'fs_read_dir_t' variable
 * @param path path to a directory
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_open(lv_fs_drv_t* drv, void* dir_p, const char* path)
{
	FRESULT res = f_opendir((dir_t*)dir_p, path);
	if (res == FR_OK) return LV_FS_RES_OK;
	else return LV_FS_RES_UNKNOWN;
}

/**
 * Read the next filename form a directory.
 * The name of the directories will begin with '/'
 * @param drv pointer to a driver where this function belongs
 * @param dir_p pointer to an initialized 'fs_read_dir_t' variable
 * @param fn pointer to a buffer to store the filename
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_read(lv_fs_drv_t* drv, void* dir_p, char* fn)
{
	FRESULT res;
	FILINFO fno;
	fn[0] = '\0';

	do
	{
		res = f_readdir((dir_t*)dir_p, &fno);
		if (res != FR_OK) return LV_FS_RES_UNKNOWN;

		if (fno.fattrib & AM_DIR)
		{
			fn[0] = '/';
			strcpy(&fn[1], fno.fname);
		}
		else strcpy(fn, fno.fname);

	} while (strcmp(fn, "/.") == 0 || strcmp(fn, "/..") == 0);

	return LV_FS_RES_OK;
}

/**
 * Close the directory reading
 * @param drv pointer to a driver where this function belongs
 * @param dir_p pointer to an initialized 'fs_read_dir_t' variable
 * @return LV_FS_RES_OK or any error from lv_fs_res_t enum
 */
static lv_fs_res_t fs_dir_close(lv_fs_drv_t* drv, void* dir_p)
{
	f_closedir((dir_t*)dir_p);
	return LV_FS_RES_OK;
}