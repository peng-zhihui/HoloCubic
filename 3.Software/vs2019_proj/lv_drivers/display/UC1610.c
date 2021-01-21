/**
 * @file UC1610.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "UC1610.h"

#if USE_UC1610

#include <stdbool.h>
#include LV_DRV_DISP_INCLUDE
#include LV_DRV_DELAY_INCLUDE

/*********************
 *      DEFINES
 *********************/
#define UC1610_CMD_MODE    0
#define UC1610_DATA_MODE   1
#define UC1610_RESET_MODE  0
#define UC1610_SET_MODE    1

/* hardware control commands */
#define UC1610_SYSTEM_RESET                 0xE2      /* software reset */
#define UC1610_NOP                          0xE3
#define UC1610_SET_TEMP_COMP                0x24      /* set temperature compensation, default -0.05%/°C */
#define UC1610_SET_PANEL_LOADING            0x29      /* set panel loading, default 16~21 nF */
#define UC1610_SET_PUMP_CONTROL             0x2F      /* default internal Vlcd (8x pump) */
#define UC1610_SET_LCD_BIAS_RATIO           0xEB      /* default 11 */
#define UC1610_SET_VBIAS_POT                0x81      /* 1 byte (0~255) to follow setting the contrast, default 0x81 */
#define UC1610_SET_LINE_RATE                0xA0      /* default 12,1 Klps */
#define UC1610_SET_DISPLAY_ENABLE           0xAE      /* + 1 / 0 : exit sleep mode / entering sleep mode */
#define UC1610_SET_LCD_GRAY_SHADE           0xD0      /* default 24% between the two gray shade levels */
#define UC1610_SET_COM_END                  0xF1      /* set the number of used com electrodes (lines number -1) */

/* ram address control */
#define UC1610_SET_AC                       0x88      /* set ram address control */
#define UC1610_AC_WA_FLAG                   1         /* automatic column/page increment wrap around (1 : cycle increment) */
#define UC1610_AC_AIO_FLAG                  (1 << 1)  /* auto increment order (0/1 : column/page increment first) */
#define UC1610_AC_PID_FLAG                  (1 << 2)  /* page address auto increment order (0/1 : +1/-1) */

/* set cursor ram address */
#define UC1610_SET_CA_LSB                   0x00      /* + 4 LSB bits */
#define UC1610_SET_CA_MSB                   0x10      /* + 4 MSB bits // MSB + LSB values range : 0~159 */
#define UC1610_SET_PA                       0x60      /* + 5 bits // values range : 0~26 */

/* display control commands */
#define UC1610_SET_FIXED_LINES              0x90      /* + 4 bits = 2xFL */
#define UC1610_SET_SCROLL_LINES_LSB         0x40      /* + 4 LSB bits scroll up display by N (7 bits) lines */
#define UC1610_SET_SCROLL_LINES_MSB         0x50      /* + 3 MSB bits */
#define UC1610_SET_ALL_PIXEL_ON             0xA4      /* + 1 / 0 : set all pixel on, reverse */
#define UC1610_SET_INVERSE_DISPLAY          0xA6      /* + 1 / 0 : inverse all data stored in ram, reverse */
#define UC1610_SET_MAPPING_CONTROL          0xC0      /* control mirorring */
#define UC1610_SET_MAPPING_CONTROL_LC_FLAG  1
#define UC1610_SET_MAPPING_CONTROL_MX_FLAG  (1 << 1)
#define UC1610_SET_MAPPING_CONTROL_MY_FLAG  (1 << 2)

/* window program mode */
#define UC1610_SET_WINDOW_PROGRAM_ENABLE    0xF8      /* + 1 / 0 : enable / disable window programming mode, */
                                                     /* reset before changing boundaries */
#define UC1610_SET_WP_STARTING_CA           0xF4      /* 1 byte to follow for column address */
#define UC1610_SET_WP_ENDING_CA             0xF6      /* 1 byte to follow for column address */
#define UC1610_SET_WP_STARTING_PA           0xF5      /* 1 byte to follow for page address   */
#define UC1610_SET_WP_ENDING_PA             0xF7      /* 1 byte to follow for page address   */

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static uint8_t cmd_buf[12];

/**********************
 *      MACROS
 **********************/

/* Return the byte bitmask of a pixel color corresponding to VDB arrangement */
#define PIXIDX(y, c)	((c) << (((y) & 3) << 1))

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void uc1610_init(void) {
  LV_DRV_DELAY_MS(12);

	/* initialization sequence */
#if UC1610_INIT_HARD_RST
  LV_DRV_DISP_RST(UC1610_RESET_MODE);                /* hardware reset */
  LV_DRV_DELAY_MS(1);
  LV_DRV_DISP_RST(UC1610_SET_MODE);
#else
  cmd_buf[0]  = UC1610_SYSTEM_RESET;                 /* software reset */
  LV_DRV_DISP_CMD_DATA(UC1610_CMD_MODE);
  LV_DRV_DISP_SPI_CS(0);
  LV_DRV_DISP_SPI_WR_ARRAY(cmd_buf, 1);
  LV_DRV_DISP_SPI_CS(1);
#endif

  LV_DRV_DELAY_MS(2);
  cmd_buf[0]  = UC1610_SET_COM_END;                  /* set com end value */
  cmd_buf[1]  = UC1610_VER_RES - 1;
  cmd_buf[2]  = UC1610_SET_PANEL_LOADING;
  cmd_buf[3]  = UC1610_SET_LCD_BIAS_RATIO;
  cmd_buf[4]  = UC1610_SET_VBIAS_POT;                /* set contrast */
  cmd_buf[5]  = (UC1610_INIT_CONTRAST * 255) / 100;
#if UC1610_TOP_VIEW
  cmd_buf[6]  = UC1610_SET_MAPPING_CONTROL         | /* top view */
                UC1610_SET_MAPPING_CONTROL_MY_FLAG |
                UC1610_SET_MAPPING_CONTROL_MX_FLAG;
#else
  cmd_buf[6]  = UC1610_SET_MAPPING_CONTROL;          /* bottom view */
#endif
  cmd_buf[7]  = UC1610_SET_SCROLL_LINES_LSB | 0;     /* set scroll line on line 0 */
  cmd_buf[8]  = UC1610_SET_SCROLL_LINES_MSB | 0;
  cmd_buf[9]  = UC1610_SET_AC | UC1610_AC_WA_FLAG;   /* set auto increment wrap around */
  cmd_buf[10] = UC1610_SET_INVERSE_DISPLAY  | 1;     /* invert colors to complies lv color system */
  cmd_buf[11] = UC1610_SET_DISPLAY_ENABLE   | 1;     /* turn display on */

  LV_DRV_DISP_CMD_DATA(UC1610_CMD_MODE);
  LV_DRV_DISP_SPI_CS(0);
  LV_DRV_DISP_SPI_WR_ARRAY(cmd_buf, 12);
  LV_DRV_DISP_SPI_CS(1);
}


void uc1610_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {
  /*Return if the area is out the screen*/
  if(area->x2 < 0) return;
  if(area->y2 < 0) return;
  if(area->x1 > UC1610_HOR_RES - 1) return;
  if(area->y1 > UC1610_VER_RES - 1) return;
	
  /*Truncate the area to the screen*/
  uint8_t act_x1 = area->x1 < 0 ? 0 : area->x1;
  uint8_t act_y1 = area->y1 < 0 ? 0 : area->y1;
  uint8_t act_x2 = area->x2 > UC1610_HOR_RES - 1 ? UC1610_HOR_RES - 1 : area->x2;
  uint8_t act_y2 = area->y2 > UC1610_VER_RES - 1 ? UC1610_VER_RES - 1 : area->y2;

  uint8_t * buf      = (uint8_t *) color_p;
  uint16_t  buf_size = (act_x2 - act_x1 + 1) * (((act_y2 - act_y1) >> 2) + 1);

  /*Set display window to fill*/
  cmd_buf[0] = UC1610_SET_WINDOW_PROGRAM_ENABLE | 0;  /* before changing boundaries */
  cmd_buf[1] = UC1610_SET_WP_STARTING_CA;
  cmd_buf[2] = act_x1;
  cmd_buf[3] = UC1610_SET_WP_ENDING_CA;
  cmd_buf[4] = act_x2;
  cmd_buf[5] = UC1610_SET_WP_STARTING_PA;
  cmd_buf[6] = act_y1 >> 2;
  cmd_buf[7] = UC1610_SET_WP_ENDING_PA;
  cmd_buf[8] = act_y2 >> 2;
  cmd_buf[9] = UC1610_SET_WINDOW_PROGRAM_ENABLE | 1;  /* entering window programming */

  LV_DRV_DISP_CMD_DATA(UC1610_CMD_MODE);
  LV_DRV_DISP_SPI_CS(0);
  LV_DRV_DISP_SPI_WR_ARRAY(cmd_buf, 10);
  LV_DRV_DISP_SPI_CS(1);

  /*Flush VDB on display memory*/
  LV_DRV_DISP_CMD_DATA(UC1610_DATA_MODE);
  LV_DRV_DISP_SPI_CS(0);
  LV_DRV_DISP_SPI_WR_ARRAY(buf, buf_size);
  LV_DRV_DISP_SPI_CS(1);

  lv_disp_flush_ready(disp_drv);
}

void uc1610_set_px_cb(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa) {
  (void) disp_drv;
  (void) opa;

  uint16_t idx = x + buf_w * (y >> 2);

  /* Convert color to depth 2 */
#if LV_COLOR_DEPTH == 1
  uint8_t color2 = color.full * 3;
#else
  uint8_t color2 = color.full >> (LV_COLOR_DEPTH - 2);
#endif

  buf[idx] &= ~PIXIDX(y, 3);      /* reset pixel color */
  buf[idx] |=  PIXIDX(y, color2); /* write new color   */
}

void uc1610_rounder_cb(lv_disp_drv_t * disp_drv, lv_area_t * area) {
  (void) disp_drv;
	
  /* Round y window to display memory page size */
  area->y1 = (area->y1 & (~3));
  area->y2 = (area->y2 & (~3)) + 3;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif
