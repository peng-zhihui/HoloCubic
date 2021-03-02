/**
 * @file ILI9341.h
 *
 */

#ifndef ILI9341_H
#define ILI9341_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#if USE_ILI9341

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#if LV_COLOR_DEPTH != 16
#error "ILI9341 currently supports 'LV_COLOR_DEPTH == 16'. Set it in lv_conf.h"
#endif

#if LV_COLOR_16_SWAP != 1
#error "ILI9341 SPI requires LV_COLOR_16_SWAP == 1. Set it in lv_conf.h"
#endif

/*********************
 *      DEFINES
 *********************/
#define ILI9341_BGR true
#define ILI9341_RGB false

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void ili9341_init(void);
void ili9341_flush(lv_disp_drv_t * drv, const lv_area_t * area, const lv_color_t * color_p);
void ili9341_rotate(int degrees, bool bgr);
/**********************
 *      MACROS
 **********************/

#endif /* USE_ILI9341 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ILI9341_H */
