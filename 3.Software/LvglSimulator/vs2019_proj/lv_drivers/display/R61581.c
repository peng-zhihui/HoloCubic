/**
 * @file R61581.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "R61581.h"
#if USE_R61581 != 0

#include <stdbool.h>
#include "lvgl/lv_core/lv_vdb.h"
#include LV_DRV_DISP_INCLUDE
#include LV_DRV_DELAY_INCLUDE

/*********************
 *      DEFINES
 *********************/
#define R61581_CMD_MODE     0
#define R61581_DATA_MODE    1

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void r61581_io_init(void);
static void r61581_reset(void);
static void r61581_set_tft_spec(void);
static inline void r61581_cmd_mode(void);
static inline void r61581_data_mode(void);
static inline void r61581_cmd(uint8_t cmd);
static inline void r61581_data(uint8_t data);

/**********************
 *  STATIC VARIABLES
 **********************/
static bool cmd_mode = true;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the R61581 display controller
 * @return HW_RES_OK or any error from hw_res_t enum
 */
void r61581_init(void)
{
    r61581_io_init();

    /*Slow mode until the PLL is not started in the display controller*/
    LV_DRV_DISP_PAR_SLOW;

    r61581_reset();

    r61581_set_tft_spec();

    r61581_cmd(0x13);       //SET display on

    r61581_cmd(0x29);       //SET display on
    LV_DRV_DELAY_MS(30);

    /*Parallel to max speed*/
    LV_DRV_DISP_PAR_FAST;
}

void r61581_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > R61581_HOR_RES - 1) return;
    if(y1 > R61581_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > R61581_HOR_RES - 1 ? R61581_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > R61581_VER_RES - 1 ? R61581_VER_RES - 1 : y2;


    //Set the rectangular area
    r61581_cmd(0x002A);
    r61581_data(act_x1 >> 8);
    r61581_data(0x00FF & act_x1);
    r61581_data(act_x2 >> 8);
    r61581_data(0x00FF & act_x2);

    r61581_cmd(0x002B);
    r61581_data(act_y1 >> 8);
    r61581_data(0x00FF & act_y1);
    r61581_data(act_y2 >> 8);
    r61581_data(0x00FF & act_y2);

    r61581_cmd(0x2c);

    int16_t i;
    uint16_t full_w = x2 - x1 + 1;

    r61581_data_mode();

#if LV_COLOR_DEPTH == 16
    uint16_t act_w = act_x2 - act_x1 + 1;
    for(i = act_y1; i <= act_y2; i++) {
        LV_DRV_DISP_PAR_WR_ARRAY((uint16_t *)color_p, act_w);
        color_p += full_w;
    }
#else
    int16_t j;
    for(i = act_y1; i <= act_y2; i++) {
        for(j = 0; j <= act_x2 - act_x1 + 1; j++) {
            LV_DRV_DISP_PAR_WR_WORD(lv_color_to16(color_p[j]));
            color_p += full_w;
        }
    }
#endif

    lv_flush_ready();
}

void r61581_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color)
{
    /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > R61581_HOR_RES - 1) return;
    if(y1 > R61581_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > R61581_HOR_RES - 1 ? R61581_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > R61581_VER_RES - 1 ? R61581_VER_RES - 1 : y2;

    //Set the rectangular area
    r61581_cmd(0x002A);
    r61581_data(act_x1 >> 8);
    r61581_data(0x00FF & act_x1);
    r61581_data(act_x2 >> 8);
    r61581_data(0x00FF & act_x2);

    r61581_cmd(0x002B);
    r61581_data(act_y1 >> 8);
    r61581_data(0x00FF & act_y1);
    r61581_data(act_y2 >> 8);
    r61581_data(0x00FF & act_y2);

    r61581_cmd(0x2c);

    r61581_data_mode();

    uint16_t color16 = lv_color_to16(color);
    uint32_t size = (act_x2 - act_x1 + 1) * (act_y2 - act_y1 + 1);
    uint32_t i;
    for(i = 0; i < size; i++) {
        LV_DRV_DISP_PAR_WR_WORD(color16);
    }
}

void r61581_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > R61581_HOR_RES - 1) return;
    if(y1 > R61581_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > R61581_HOR_RES - 1 ? R61581_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > R61581_VER_RES - 1 ? R61581_VER_RES - 1 : y2;


    //Set the rectangular area
    r61581_cmd(0x002A);
    r61581_data(act_x1 >> 8);
    r61581_data(0x00FF & act_x1);
    r61581_data(act_x2 >> 8);
    r61581_data(0x00FF & act_x2);

    r61581_cmd(0x002B);
    r61581_data(act_y1 >> 8);
    r61581_data(0x00FF & act_y1);
    r61581_data(act_y2 >> 8);
    r61581_data(0x00FF & act_y2);

    r61581_cmd(0x2c);

    int16_t i;
    uint16_t full_w = x2 - x1 + 1;

    r61581_data_mode();

#if LV_COLOR_DEPTH == 16
    uint16_t act_w = act_x2 - act_x1 + 1;
    for(i = act_y1; i <= act_y2; i++) {
        LV_DRV_DISP_PAR_WR_ARRAY((uint16_t *)color_p, act_w);
        color_p += full_w;
    }
#else
    int16_t j;
    for(i = act_y1; i <= act_y2; i++) {
        for(j = 0; j <= act_x2 - act_x1 + 1; j++) {
            LV_DRV_DISP_PAR_WR_WORD(lv_color_to16(color_p[j]));
            color_p += full_w;
        }
    }
#endif
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Io init
 */
static void r61581_io_init(void)
{
    LV_DRV_DISP_CMD_DATA(R61581_CMD_MODE)
    cmd_mode = true;
}

/**
 * Reset
 */
static void r61581_reset(void)
{
    /*Hardware reset*/
    LV_DRV_DISP_RST(1);
    LV_DRV_DELAY_MS(50);
    LV_DRV_DISP_RST(0);
    LV_DRV_DELAY_MS(50);
    LV_DRV_DISP_RST(1);
    LV_DRV_DELAY_MS(50);

    /*Chip enable*/
    LV_DRV_DISP_PAR_CS(1);
    LV_DRV_DELAY_MS(10);
    LV_DRV_DISP_PAR_CS(0);
    LV_DRV_DELAY_MS(5);

    /*Software reset*/
    r61581_cmd(0x01);
    LV_DRV_DELAY_MS(20);

    r61581_cmd(0x01);
    LV_DRV_DELAY_MS(20);

    r61581_cmd(0x01);
    LV_DRV_DELAY_MS(20);
}

/**
 * TFT specific initialization
 */
static void r61581_set_tft_spec(void)
{
    r61581_cmd(0xB0);
    r61581_data(0x00);

    r61581_cmd(0xB3);
    r61581_data(0x02);
    r61581_data(0x00);
    r61581_data(0x00);
    r61581_data(0x10);

    r61581_cmd(0xB4);
    r61581_data(0x00);//0X10

    r61581_cmd(0xB9); //PWM
    r61581_data(0x01);
    r61581_data(0xFF); //FF brightness
    r61581_data(0xFF);
    r61581_data(0x18);

    /*Panel Driving Setting*/
    r61581_cmd(0xC0);
    r61581_data(0x02);
    r61581_data(0x3B);
    r61581_data(0x00);
    r61581_data(0x00);
    r61581_data(0x00);
    r61581_data(0x01);
    r61581_data(0x00);//NW
    r61581_data(0x43);

    /*Display Timing Setting for Normal Mode */
    r61581_cmd(0xC1);
    r61581_data(0x08);
    r61581_data(0x15);  //CLOCK
    r61581_data(R61581_VFP);
    r61581_data(R61581_VBP);

    /*Source/VCOM/Gate Driving Timing Setting*/
    r61581_cmd(0xC4);
    r61581_data(0x15);
    r61581_data(0x03);
    r61581_data(0x03);
    r61581_data(0x01);

    /*Interface Setting*/
    r61581_cmd(0xC6);
    r61581_data((R61581_DPL << 0) |
                (R61581_EPL << 1) |
                (R61581_HSPL << 4) |
                (R61581_VSPL << 5));

    /*Gamma Set*/
    r61581_cmd(0xC8);
    r61581_data(0x0c);
    r61581_data(0x05);
    r61581_data(0x0A);
    r61581_data(0x6B);
    r61581_data(0x04);
    r61581_data(0x06);
    r61581_data(0x15);
    r61581_data(0x10);
    r61581_data(0x00);
    r61581_data(0x31);


    r61581_cmd(0x36);
    if(R61581_ORI == 0) r61581_data(0xE0);
    else r61581_data(0x20);

    r61581_cmd(0x0C);
    r61581_data(0x55);

    r61581_cmd(0x3A);
    r61581_data(0x55);

    r61581_cmd(0x38);

    r61581_cmd(0xD0);
    r61581_data(0x07);
    r61581_data(0x07);
    r61581_data(0x14);
    r61581_data(0xA2);

    r61581_cmd(0xD1);
    r61581_data(0x03);
    r61581_data(0x5A);
    r61581_data(0x10);

    r61581_cmd(0xD2);
    r61581_data(0x03);
    r61581_data(0x04);
    r61581_data(0x04);

    r61581_cmd(0x11);
    LV_DRV_DELAY_MS(10);

    r61581_cmd(0x2A);
    r61581_data(0x00);
    r61581_data(0x00);
    r61581_data(((R61581_HOR_RES - 1) >> 8) & 0XFF);
    r61581_data((R61581_HOR_RES - 1) & 0XFF);

    r61581_cmd(0x2B);
    r61581_data(0x00);
    r61581_data(0x00);
    r61581_data(((R61581_VER_RES - 1) >> 8) & 0XFF);
    r61581_data((R61581_VER_RES - 1) & 0XFF);

    LV_DRV_DELAY_MS(10);

    r61581_cmd(0x29);
    LV_DRV_DELAY_MS(5);

    r61581_cmd(0x2C);
    LV_DRV_DELAY_MS(5);
}

/**
 * Command mode
 */
static inline void r61581_cmd_mode(void)
{
    if(cmd_mode == false) {
        LV_DRV_DISP_CMD_DATA(R61581_CMD_MODE)
        cmd_mode = true;
    }
}

/**
 * Data mode
 */
static inline void r61581_data_mode(void)
{
    if(cmd_mode != false) {
        LV_DRV_DISP_CMD_DATA(R61581_DATA_MODE);
        cmd_mode = false;
    }
}

/**
 * Write command
 * @param cmd the command
 */
static inline void r61581_cmd(uint8_t cmd)
{
    r61581_cmd_mode();
    LV_DRV_DISP_PAR_WR_WORD(cmd);
}

/**
 * Write data
 * @param data the data
 */
static inline void r61581_data(uint8_t data)
{
    r61581_data_mode();
    LV_DRV_DISP_PAR_WR_WORD(data);
}
#endif
