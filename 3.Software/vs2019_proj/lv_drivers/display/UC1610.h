/**
 * @file UC1610.h
 *
 */

#ifndef UC1610_H
#define UC1610_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#ifndef LV_DRV_NO_CONF
#ifdef LV_CONF_INCLUDE_SIMPLE
#include "lv_drv_conf.h"
#else
#include "../../lv_drv_conf.h"
#endif
#endif

#if USE_UC1610

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void uc1610_init(void);
void uc1610_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void uc1610_rounder_cb(lv_disp_drv_t * disp_drv, lv_area_t * area);
void uc1610_set_px_cb(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa);

/**********************
 *      MACROS
 **********************/

#endif /* USE_UC1610 */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* UC1610_H */
