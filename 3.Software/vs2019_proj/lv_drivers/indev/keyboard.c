/**
 * @file sdl_kb.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "keyboard.h"
#if USE_KEYBOARD

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static uint32_t keycode_to_ascii(uint32_t sdl_key);

/**********************
 *  STATIC VARIABLES
 **********************/
static uint32_t last_key;
static lv_indev_state_t state;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the keyboard
 */
void keyboard_init(void)
{
    /*Nothing to init*/
}

/**
 * Get the last pressed or released character from the PC's keyboard
 * @param indev_drv pointer to the related input device driver
 * @param data store the read data here
 * @return false: because the points are not buffered, so no more data to be read
 */
bool keyboard_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    (void) indev_drv;      /*Unused*/
    data->state = state;
    data->key = keycode_to_ascii(last_key);

    return false;
}

/**
 * It is called periodically from the SDL thread to check a key is pressed/released
 * @param event describes the event
 */
void keyboard_handler(SDL_Event * event)
{
    /* We only care about SDL_KEYDOWN and SDL_KEYUP events */
    switch(event->type) {
        case SDL_KEYDOWN:                       /*Button press*/
            last_key = event->key.keysym.sym;   /*Save the pressed key*/
            state = LV_INDEV_STATE_PR;          /*Save the key is pressed now*/
            break;
        case SDL_KEYUP:                         /*Button release*/
            state = LV_INDEV_STATE_REL;         /*Save the key is released but keep the last key*/
            break;
        default:
            break;

    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Convert the key code LV_KEY_... "codes" or leave them if they are not control characters
 * @param sdl_key the key code
 * @return
 */
static uint32_t keycode_to_ascii(uint32_t sdl_key)
{
    /*Remap some key to LV_KEY_... to manage groups*/
    switch(sdl_key) {
        case SDLK_RIGHT:
        case SDLK_KP_PLUS:
            return LV_KEY_RIGHT;

        case SDLK_LEFT:
        case SDLK_KP_MINUS:
            return LV_KEY_LEFT;

        case SDLK_UP:
            return LV_KEY_UP;

        case SDLK_DOWN:
            return LV_KEY_DOWN;

        case SDLK_ESCAPE:
            return LV_KEY_ESC;

#ifdef  LV_KEY_BACKSPACE        /*For backward compatibility*/
        case SDLK_BACKSPACE:
            return LV_KEY_BACKSPACE;
#endif

#ifdef  LV_KEY_DEL        /*For backward compatibility*/
        case SDLK_DELETE:
            return LV_KEY_DEL;
#endif
        case SDLK_KP_ENTER:
        case '\r':
            return LV_KEY_ENTER;

        default:
            return sdl_key;
    }
}
#endif
