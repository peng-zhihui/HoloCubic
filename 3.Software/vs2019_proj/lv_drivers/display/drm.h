/**
 * @file drm.h
 *
 */

#ifndef DRM_H
#define DRM_H

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

#if USE_DRM

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
void drm_init(void);
void drm_get_sizes(lv_coord_t *width, lv_coord_t *height, uint32_t *dpi);
void drm_exit(void);
void drm_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p);
void drm_wait_vsync(lv_disp_drv_t * drv);


/**********************
 *      MACROS
 **********************/

#endif  /*USE_DRM*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DRM_H*/
