/**
 * @file SHARP_MIP.c
 *
 */

/*-------------------------------------------------------------------------------------------------
 *  SHARP memory in pixel monochrome display series
 *      LS012B7DD01 (184x38  pixels.)
 *      LS013B7DH03 (128x128 pixels.)
 *      LS013B7DH05 (144x168 pixels.)
 *      LS027B7DH01 (400x240 pixels.) (tested)
 *      LS032B7DD02 (336x536 pixels.)
 *      LS044Q7DH01 (320x240 pixels.)
 *
 *  These displays need periodic com inversion, there are two ways :
 *    - software com inversion :
 *      define SHARP_MIP_SOFT_COM_INVERSION 1 and set EXTMODE display pin LOW,
 *      call sharp_mip_com_inversion() periodically
 *    - hardware com inversion with EXTCOMIN display pin :
 *      define SHARP_MIP_SOFT_COM_INVERSION 0,
 *      set EXTMODE display pin HIGH and handle
 *      EXTCOMIN waveform (for example with mcu pwm output),
 *      see datasheet pages 8-12 for details
 *
 *  VDB size : (LV_VER_RES / X) * (2 + LV_HOR_RES / 8) + 2 bytes, structure :
 *      [FRAME_HEADER (1 byte)] [GATE_ADDR (1 byte )] [LINE_DATA (LV_HOR_RES / 8 bytes)]  1st  line
 *      [DUMMY        (1 byte)] [GATE_ADDR (1 byte )] [LINE_DATA (LV_HOR_RES / 8 bytes)]  2nd  line
 *      ...........................................................................................
 *      [DUMMY        (1 byte)] [GATE_ADDR (1 byte )] [LINE_DATA (LV_HOR_RES / 8 bytes)]  last line
 *      [DUMMY                             (2 bytes)]
 *
 *  Since extra bytes (dummy, addresses, header) are stored in VDB, we need to use
 *  an "oversized" VDB. Buffer declaration in "lv_port_disp.c" becomes for example :
 *      static lv_disp_buf_t disp_buf;
 *      static uint8_t buf[(LV_VER_RES_MAX / X) * (2 + (LV_HOR_RES_MAX / 8)) + 2];
 *      lv_disp_buf_init(&disp_buf, buf, NULL, LV_VER_RES_MAX * LV_HOR_RES_MAX / X);
 *-----------------------------------------------------------------------------------------------*/

/*********************
 *      INCLUDES
 *********************/

#include "SHARP_MIP.h"

#if USE_SHARP_MIP

#include <stdbool.h>
#include LV_DRV_DISP_INCLUDE
#include LV_DRV_DELAY_INCLUDE

/*********************
 *      DEFINES
 *********************/

#define SHARP_MIP_HEADER              0
#define SHARP_MIP_UPDATE_RAM_FLAG     (1 << 7)  /* (M0) Mode flag : H -> update memory, L -> maintain memory */
#define SHARP_MIP_COM_INVERSION_FLAG  (1 << 6)  /* (M1) Frame inversion flag : relevant when EXTMODE = L,    */
                                                /*      H -> outputs VCOM = H, L -> outputs VCOM = L         */
#define SHARP_MIP_CLEAR_SCREEN_FLAG   (1 << 5)  /* (M2) All clear flag : H -> clear all pixels               */

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

#if SHARP_MIP_SOFT_COM_INVERSION
static bool_t com_output_state = false;
#endif

/**********************
 *      MACROS
 **********************/

/*
 * Return the VDB byte index corresponding to the pixel
 * relatives coordinates (x, y) in the area.
 * The area is rounded to a whole screen line.
 */
#define BUFIDX(x, y)  (((x) >> 3) + ((y) * (2 + (SHARP_MIP_HOR_RES >> 3))) + 2)

/*
 * Return the byte bitmask of a pixel bit corresponding
 * to VDB arrangement (8 pixels per byte on lines).
 */
#define PIXIDX(x)     SHARP_MIP_REV_BYTE(1 << ((x) & 7))

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void sharp_mip_init(void) {
  /* These displays have nothing to initialize */
}


void sharp_mip_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p) {

  /*Return if the area is out the screen*/
  if(area->y2 < 0) return;
  if(area->y1 > SHARP_MIP_VER_RES - 1) return;

  /*Truncate the area to the screen*/
  uint16_t act_y1 = area->y1 < 0 ? 0 : area->y1;
  uint16_t act_y2 = area->y2 > SHARP_MIP_VER_RES - 1 ? SHARP_MIP_VER_RES - 1 : area->y2;

  uint8_t * buf      = (uint8_t *) color_p;                     /*Get the buffer address*/
  uint16_t  buf_h    = (act_y2 - act_y1 + 1);                   /*Number of buffer lines*/
  uint16_t  buf_size = buf_h * (2 + SHARP_MIP_HOR_RES / 8) + 2; /*Buffer size in bytes  */

  /* Set lines to flush dummy byte & gate address in VDB*/
  for(uint16_t act_y = 0 ; act_y < buf_h ; act_y++) {
    buf[BUFIDX(0, act_y) - 1] = SHARP_MIP_REV_BYTE((act_y1 + act_y + 1));
    buf[BUFIDX(0, act_y) - 2] = 0;
  }

  /* Set last dummy two bytes in VDB */
  buf[BUFIDX(0, buf_h) - 1] = 0;
  buf[BUFIDX(0, buf_h) - 2] = 0;

  /* Set frame header in VDB */
  buf[0] = SHARP_MIP_HEADER         |
           SHARP_MIP_UPDATE_RAM_FLAG;

  /* Write the frame on display memory */
  LV_DRV_DISP_SPI_CS(1);
  LV_DRV_DISP_SPI_WR_ARRAY(buf, buf_size);
  LV_DRV_DISP_SPI_CS(0);

  lv_disp_flush_ready(disp_drv);
}

void sharp_mip_set_px(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa) {
  (void) disp_drv;
  (void) buf_w;
  (void) opa;

  if (lv_color_to1(color) != 0) {
    buf[BUFIDX(x, y)] |=  PIXIDX(x);  /*Set VDB pixel bit to 1 for other colors than BLACK*/
  } else {
    buf[BUFIDX(x, y)] &= ~PIXIDX(x);  /*Set VDB pixel bit to 0 for BLACK color*/
  }
}

void sharp_mip_rounder(lv_disp_drv_t * disp_drv, lv_area_t * area) {
  (void) disp_drv;

  /* Round area to a whole line */
  area->x1 = 0;
  area->x2 = SHARP_MIP_HOR_RES - 1;
}

#if SHARP_MIP_SOFT_COM_INVERSION
void sharp_mip_com_inversion(void) {
  uint8_t inversion_header[2] = {0};

  /* Set inversion header */
  if (com_output_state) {
    com_output_state = false;
  } else {
    inversion_header[0] |= SHARP_MIP_COM_INVERSION_FLAG;
    com_output_state = true;
  }

  /* Write inversion header on display memory */
  LV_DRV_DISP_SPI_CS(1);
  LV_DRV_DISP_SPI_WR_ARRAY(inversion_header, 2);
  LV_DRV_DISP_SPI_CS(0);
}
#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif
