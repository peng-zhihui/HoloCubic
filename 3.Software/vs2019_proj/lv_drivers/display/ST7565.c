/**
 * @file ST7565.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ST7565.h"
#if USE_ST7565

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "lvgl/lv_core/lv_vdb.h"
#include LV_DRV_DISP_INCLUDE
#include LV_DRV_DELAY_INCLUDE

/*********************
 *      DEFINES
 *********************/
#define ST7565_BAUD      2000000    /*< 2,5 MHz (400 ns)*/

#define ST7565_CMD_MODE  0
#define ST7565_DATA_MODE 1

#define ST7565_HOR_RES  128
#define ST7565_VER_RES  64

#define CMD_DISPLAY_OFF         0xAE
#define CMD_DISPLAY_ON          0xAF

#define CMD_SET_DISP_START_LINE 0x40
#define CMD_SET_PAGE            0xB0

#define CMD_SET_COLUMN_UPPER    0x10
#define CMD_SET_COLUMN_LOWER    0x00

#define CMD_SET_ADC_NORMAL      0xA0
#define CMD_SET_ADC_REVERSE     0xA1

#define CMD_SET_DISP_NORMAL     0xA6
#define CMD_SET_DISP_REVERSE    0xA7

#define CMD_SET_ALLPTS_NORMAL   0xA4
#define CMD_SET_ALLPTS_ON       0xA5
#define CMD_SET_BIAS_9          0xA2
#define CMD_SET_BIAS_7          0xA3

#define CMD_RMW                 0xE0
#define CMD_RMW_CLEAR           0xEE
#define CMD_INTERNAL_RESET      0xE2
#define CMD_SET_COM_NORMAL      0xC0
#define CMD_SET_COM_REVERSE     0xC8
#define CMD_SET_POWER_CONTROL   0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST    0x81
#define CMD_SET_VOLUME_SECOND   0x00
#define CMD_SET_STATIC_OFF      0xAC
#define CMD_SET_STATIC_ON       0xAD
#define CMD_SET_STATIC_REG      0x00
#define CMD_SET_BOOSTER_FIRST   0xF8
#define CMD_SET_BOOSTER_234     0x00
#define CMD_SET_BOOSTER_5       0x01
#define CMD_SET_BOOSTER_6       0x03
#define CMD_NOP                 0xE3
#define CMD_TEST                0xF0

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void st7565_sync(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
static void st7565_command(uint8_t cmd);
static void st7565_data(uint8_t data);

/**********************
 *  STATIC VARIABLES
 **********************/
static uint8_t lcd_fb[ST7565_HOR_RES * ST7565_VER_RES / 8] = {0xAA, 0xAA};
static uint8_t pagemap[] = { 7, 6, 5, 4, 3, 2, 1, 0 };

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the ST7565
 */
void st7565_init(void)
{
    LV_DRV_DISP_RST(1);
    LV_DRV_DELAY_MS(10);
    LV_DRV_DISP_RST(0);
    LV_DRV_DELAY_MS(10);
    LV_DRV_DISP_RST(1);
    LV_DRV_DELAY_MS(10);

    LV_DRV_DISP_SPI_CS(0);

    st7565_command(CMD_SET_BIAS_7);
    st7565_command(CMD_SET_ADC_NORMAL);
    st7565_command(CMD_SET_COM_NORMAL);
    st7565_command(CMD_SET_DISP_START_LINE);
    st7565_command(CMD_SET_POWER_CONTROL | 0x4);
    LV_DRV_DELAY_MS(50);

    st7565_command(CMD_SET_POWER_CONTROL | 0x6);
    LV_DRV_DELAY_MS(50);

    st7565_command(CMD_SET_POWER_CONTROL | 0x7);
    LV_DRV_DELAY_MS(10);

    st7565_command(CMD_SET_RESISTOR_RATIO | 0x6); // Defaulted to 0x26 (but could also be between 0x20-0x27 based on display's specs)

    st7565_command(CMD_DISPLAY_ON);
    st7565_command(CMD_SET_ALLPTS_NORMAL);

    /*Set brightness*/
    st7565_command(CMD_SET_VOLUME_FIRST);
    st7565_command(CMD_SET_VOLUME_SECOND | (0x18 & 0x3f));

    LV_DRV_DISP_SPI_CS(1);

    memset(lcd_fb, 0x00, sizeof(lcd_fb));
}


void st7565_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > ST7565_HOR_RES - 1) return;
    if(y1 > ST7565_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > ST7565_HOR_RES - 1 ? ST7565_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > ST7565_VER_RES - 1 ? ST7565_VER_RES - 1 : y2;

    int32_t x, y;

    /*Set the first row in */

    /*Refresh frame buffer*/
    for(y = act_y1; y <= act_y2; y++) {
        for(x = act_x1; x <= act_x2; x++) {
            if(lv_color_to1(*color_p) != 0) {
                lcd_fb[x + (y / 8)*ST7565_HOR_RES] &= ~(1 << (7 - (y % 8)));
            } else {
                lcd_fb[x + (y / 8)*ST7565_HOR_RES] |= (1 << (7 - (y % 8)));
            }
            color_p ++;
        }

        color_p += x2 - act_x2; /*Next row*/
    }

    st7565_sync(act_x1, act_y1, act_x2, act_y2);
    lv_flush_ready();
}



void st7565_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color)
{
    /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > ST7565_HOR_RES - 1) return;
    if(y1 > ST7565_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > ST7565_HOR_RES - 1 ? ST7565_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > ST7565_VER_RES - 1 ? ST7565_VER_RES - 1 : y2;

    int32_t x, y;
    uint8_t white = lv_color_to1(color);

    /*Refresh frame buffer*/
    for(y = act_y1; y <= act_y2; y++) {
        for(x = act_x1; x <= act_x2; x++) {
            if(white != 0) {
                lcd_fb[x + (y / 8)*ST7565_HOR_RES] |= (1 << (7 - (y % 8)));
            } else {
                lcd_fb[x + (y / 8)*ST7565_HOR_RES] &= ~(1 << (7 - (y % 8)));
            }
        }
    }

    st7565_sync(act_x1, act_y1, act_x2, act_y2);
}

void st7565_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*Return if the area is out the screen*/
    if(x2 < 0) return;
    if(y2 < 0) return;
    if(x1 > ST7565_HOR_RES - 1) return;
    if(y1 > ST7565_VER_RES - 1) return;

    /*Truncate the area to the screen*/
    int32_t act_x1 = x1 < 0 ? 0 : x1;
    int32_t act_y1 = y1 < 0 ? 0 : y1;
    int32_t act_x2 = x2 > ST7565_HOR_RES - 1 ? ST7565_HOR_RES - 1 : x2;
    int32_t act_y2 = y2 > ST7565_VER_RES - 1 ? ST7565_VER_RES - 1 : y2;

    int32_t x, y;

    /*Set the first row in */

    /*Refresh frame buffer*/
    for(y = act_y1; y <= act_y2; y++) {
        for(x = act_x1; x <= act_x2; x++) {
            if(lv_color_to1(*color_p) != 0) {
                lcd_fb[x + (y / 8)*ST7565_HOR_RES] &= ~(1 << (7 - (y % 8)));
            } else {
                lcd_fb[x + (y / 8)*ST7565_HOR_RES] |= (1 << (7 - (y % 8)));
            }
            color_p ++;
        }

        color_p += x2 - act_x2; /*Next row*/
    }

    st7565_sync(act_x1, act_y1, act_x2, act_y2);
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
/**
 * Flush a specific part of the buffer to the display
 * @param x1 left coordinate of the area to flush
 * @param y1 top coordinate of the area to flush
 * @param x2 right coordinate of the area to flush
 * @param y2 bottom coordinate of the area to flush
 */
static void st7565_sync(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{

    LV_DRV_DISP_SPI_CS(0);

    uint8_t c, p;
    for(p = y1 / 8; p <= y2 / 8; p++) {
        st7565_command(CMD_SET_PAGE | pagemap[p]);
        st7565_command(CMD_SET_COLUMN_LOWER | (x1 & 0xf));
        st7565_command(CMD_SET_COLUMN_UPPER | ((x1 >> 4) & 0xf));
        st7565_command(CMD_RMW);

        for(c = x1; c <= x2; c++) {
            st7565_data(lcd_fb[(ST7565_HOR_RES * p) + c]);
        }
    }

    LV_DRV_DISP_SPI_CS(1);
}

/**
 * Write a command to the ST7565
 * @param cmd the command
 */
static void st7565_command(uint8_t cmd)
{
    LV_DRV_DISP_CMD_DATA(ST7565_CMD_MODE);
    LV_DRV_DISP_SPI_WR_BYTE(cmd);
}

/**
 * Write data to the ST7565
 * @param data the data
 */
static void st7565_data(uint8_t data)
{
    LV_DRV_DISP_CMD_DATA(ST7565_DATA_MODE);
    LV_DRV_DISP_SPI_WR_BYTE(data);
}

#endif
