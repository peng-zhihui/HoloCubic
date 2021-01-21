/**
 * @file SHARP_MIP.h
 *
 */

#ifndef SHARP_MIP_H
#define SHARP_MIP_H

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

#if USE_SHARP_MIP

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
void sharp_mip_init(void);
void sharp_mip_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
void sharp_mip_rounder(lv_disp_drv_t * disp_drv, lv_area_t * area);
void sharp_mip_set_px(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa);
#if SHARP_MIP_SOFT_COM_INVERSION
void sharp_mip_com_inversion(void);
#endif

/**********************
 *      MACROS
 **********************/

#endif /* USE_SHARP_MIP */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SHARP_MIP_H */
