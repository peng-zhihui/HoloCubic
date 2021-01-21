/**
 * @file mousewheel.h
 *
 */

#ifndef MOUSEWHEEL_H
#define MOUSEWHEEL_H

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

#if USE_MOUSEWHEEL

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef MONITOR_SDL_INCLUDE_PATH
#define MONITOR_SDL_INCLUDE_PATH <SDL2/SDL.h>
#endif

#include MONITOR_SDL_INCLUDE_PATH

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Initialize the encoder
 */
void mousewheel_init(void);

/**
 * Get encoder (i.e. mouse wheel) ticks difference and pressed state
 * @param indev_drv pointer to the related input device driver
 * @param data store the read data here
 * @return false: all ticks and button state are handled
 */
bool mousewheel_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

/**
 * It is called periodically from the SDL thread to check a key is pressed/released
 * @param event describes the event
 */
void mousewheel_handler(SDL_Event *event);

/**********************
 *      MACROS
 **********************/

#endif /*USE_MOUSEWHEEL*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*MOUSEWHEEL_H*/
