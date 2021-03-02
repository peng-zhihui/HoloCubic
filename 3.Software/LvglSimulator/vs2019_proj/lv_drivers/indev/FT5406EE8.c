/**
 * @file FT5406EE8.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "FT5406EE8.h"
#if USE_FT5406EE8

#include <stddef.h>
#include <stdbool.h>
#include LV_DRV_INDEV_INCLUDE
#include LV_DRV_DELAY_INCLUDE

/*********************
 *      DEFINES
 *********************/

#define I2C_WR_BIT    0x00
#define I2C_RD_BIT    0x01

/*DEVICE MODES*/
#define OPERAT_MD   0x00
#define TEST_MD     0x04
#define SYS_INF_MD  0x01

/*OPERATING MODE*/
#define DEVICE_MODE 0x00
#define GEST_ID     0x01
#define TD_STATUS   0x02

#define FT5406EE8_FINGER_MAX 10

/*Register adresses*/
#define FT5406EE8_REG_DEVICE_MODE 0x00
#define FT5406EE8_REG_GEST_ID     0x01
#define FT5406EE8_REG_TD_STATUS   0x02
#define FT5406EE8_REG_YH          0x03
#define FT5406EE8_REG_YL          0x04
#define FT5406EE8_REG_XH          0x05
#define FT5406EE8_REG_XL          0x06

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static bool ft5406ee8_get_touch_num(void);
static bool ft5406ee8_read_finger1(int16_t * x,  int16_t * y);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 *
 */
void ft5406ee8_init(void)
{

}

/**
 * Get the current position and state of the touchpad
 * @param data store the read data here
 * @return false: because no ore data to be read
 */
bool ft5406ee8_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static int16_t x_last;
    static int16_t y_last;
    int16_t x;
    int16_t y;
    bool valid = true;

    valid = ft5406ee8_get_touch_num();
    if(valid == true) {
        valid = ft5406ee8_read_finger1(&x, &y);
    }

    if(valid == true) {
        x = (uint32_t)((uint32_t)x * 320) / 2048;
        y = (uint32_t)((uint32_t)y * 240) / 2048;


        x_last = x;
        y_last = y;
    } else {
        x = x_last;
        y = y_last;
    }

    data->point.x = x;
    data->point.y = y;
    data->state = valid == false ? LV_INDEV_STATE_REL : LV_INDEV_STATE_PR;
    return false;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static bool ft5406ee8_get_touch_num(void)
{
    bool ok = true;
    uint8_t t_num = 0;

    LV_DRV_INDEV_I2C_START;
    LV_DRV_INDEV_I2C_WR((FT5406EE8_I2C_ADR << 1) | I2C_WR_BIT);
    LV_DRV_INDEV_I2C_WR(FT5406EE8_REG_TD_STATUS)
    LV_DRV_INDEV_I2C_RESTART;
    LV_DRV_INDEV_I2C_WR((FT5406EE8_I2C_ADR << 1) | I2C_RD_BIT);
    t_num = LV_DRV_INDEV_I2C_READ(0);

    /* Error if not touched or too much finger */
    if(t_num > FT5406EE8_FINGER_MAX || t_num == 0) {
        ok = false;
    }

    return ok;
}

/**
 * Read the x and y coordinated
 * @param x store the x coordinate here
 * @param y store the y coordinate here
 * @return false: not valid point; true: valid point
 */
static bool ft5406ee8_read_finger1(int16_t * x, int16_t * y)
{
    uint8_t temp_xH = 0;
    uint8_t temp_xL = 0;
    uint8_t temp_yH = 0;
    uint8_t temp_yL = 0;

    /*Read Y High and low byte*/
    LV_DRV_INDEV_I2C_START;
    LV_DRV_INDEV_I2C_WR((FT5406EE8_I2C_ADR << 1) | I2C_WR_BIT);
    LV_DRV_INDEV_I2C_WR(FT5406EE8_REG_YH)
    LV_DRV_INDEV_I2C_RESTART;
    LV_DRV_INDEV_I2C_WR((FT5406EE8_I2C_ADR << 1) | I2C_RD_BIT);
    temp_yH = LV_DRV_INDEV_I2C_READ(1);
    temp_yL = LV_DRV_INDEV_I2C_READ(1);

    /*The upper two bit must be 2 on valid press*/
    if(((temp_yH >> 6) & 0xFF) != 2) {
        (void) LV_DRV_INDEV_I2C_READ(0);   /*Dummy read to close read sequence*/
        *x = 0;
        *y = 0;
        return false;
    }

    /*Read X High and low byte*/
    temp_xH = LV_DRV_INDEV_I2C_READ(1);
    temp_xL = LV_DRV_INDEV_I2C_READ(0);

    /*Save the result*/
    *x = (temp_xH & 0x0F) << 8;
    *x += temp_xL;
    *y = (temp_yH & 0x0F) << 8;
    *y += temp_yL;

    return true;
}

#endif
