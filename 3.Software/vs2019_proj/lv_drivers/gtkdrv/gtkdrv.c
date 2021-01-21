/**
 * @file gtk.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "gtkdrv.h"

#if USE_GTK
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void gtkdrv_handler(void * p);
static gboolean mouse_pressed(GtkWidget *widget, GdkEventButton *event,
    gpointer user_data);
static gboolean mouse_released(GtkWidget *widget, GdkEventButton *event,
    gpointer user_data);
static gboolean mouse_motion(GtkWidget *widget, GdkEventMotion *event,
    gpointer user_data);
static gboolean keyboard_press(GtkWidget *widget, GdkEventKey *event,
    gpointer user_data);
static gboolean keyboard_release(GtkWidget *widget, GdkEventKey *event,
    gpointer user_data);

static void quit_handler(void);

/**********************
 *  STATIC VARIABLES
 **********************/
static GtkWidget    *window;
static GtkWidget    *event_box;

static GtkWidget    *output_image;
static GdkPixbuf    *pixbuf;

static unsigned char run_gtk;

static lv_coord_t mouse_x;
static lv_coord_t mouse_y;
static lv_indev_state_t mouse_btn = LV_INDEV_STATE_REL;
static lv_key_t last_key;
static lv_indev_state_t last_key_state;

static uint8_t fb[LV_HOR_RES_MAX * LV_VER_RES_MAX * 3];

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void gtkdrv_init(void)
{
    // Init GTK
    gtk_init(NULL, NULL);

    /* Or just set up the widgets in code */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), LV_HOR_RES_MAX, LV_VER_RES_MAX);
    gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
    output_image = gtk_image_new();
    event_box = gtk_event_box_new (); // Use event_box around image, otherwise mouse position output in broadway is offset
    gtk_container_add(GTK_CONTAINER (event_box), output_image);
    gtk_container_add(GTK_CONTAINER (window), event_box);

    gtk_widget_add_events(event_box, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(event_box, GDK_SCROLL_MASK);
    gtk_widget_add_events(event_box, GDK_POINTER_MOTION_MASK);
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);

    g_signal_connect(window, "destroy", G_CALLBACK(quit_handler), NULL);
    g_signal_connect(event_box, "button-press-event", G_CALLBACK(mouse_pressed), NULL);
    g_signal_connect(event_box, "button-release-event", G_CALLBACK(mouse_released), NULL);
    g_signal_connect(event_box, "motion-notify-event", G_CALLBACK(mouse_motion), NULL);
    g_signal_connect(window, "key_press_event", G_CALLBACK(keyboard_press), NULL);
    g_signal_connect(window, "key_release_event", G_CALLBACK(keyboard_release), NULL);


    gtk_widget_show_all(window);

    pixbuf = gdk_pixbuf_new_from_data((guchar*)fb, GDK_COLORSPACE_RGB, false, 8, LV_HOR_RES_MAX, LV_VER_RES_MAX, LV_HOR_RES_MAX * 3, NULL, NULL);
    if (pixbuf == NULL)
    {
        fprintf(stderr, "Creating pixbuf failed\n");
        return;
    }

    pthread_t thread;
    pthread_create(&thread, NULL, gtkdrv_handler, NULL);
}


/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t gtkdrv_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;

    return time_ms;
}


/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixel to copy to the `area` part of the screen
 */
void gtkdrv_flush_cb(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    lv_coord_t hres = disp_drv->rotated == 0 ? disp_drv->hor_res : disp_drv->ver_res;
    lv_coord_t vres = disp_drv->rotated == 0 ? disp_drv->ver_res : disp_drv->hor_res;


    /*Return if the area is out the screen*/
    if(area->x2 < 0 || area->y2 < 0 || area->x1 > hres - 1 || area->y1 > vres - 1) {

        lv_disp_flush_ready(disp_drv);
        return;
    }

    int32_t y;
    int32_t x;
    int32_t p;
    for(y = area->y1; y <= area->y2 && y < disp_drv->ver_res; y++) {
        p = (y * disp_drv->hor_res + area->x1) * 3;
        for(x = area->x1; x <= area->x2 && x < disp_drv->hor_res; x++) {
            fb[p] = color_p->ch.red;
            fb[p + 1] = color_p->ch.green;
            fb[p + 2] = color_p->ch.blue;

            p += 3;
            color_p ++;
        }
    }

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);
}


bool gtkdrv_mouse_read_cb(lv_indev_drv_t * drv, lv_indev_data_t * data)
{
    data->point.x = mouse_x;
    data->point.y = mouse_y;
    data->state = mouse_btn;

    return false;
}


bool gtkdrv_keyboard_read_cb(lv_indev_drv_t * drv, lv_indev_data_t * data)
{
    data->key = last_key;
    data->state = last_key_state;

    return false;
}




/**********************
 *   STATIC FUNCTIONS
 **********************/

static void gtkdrv_handler(void * p)
{
    while(1) {
        gtk_image_set_from_pixbuf(GTK_IMAGE(output_image), pixbuf); // Test code

        /* Real code should: call gdk_pixbuf_new_from_data () with pointer to frame buffer
        generated by LVGL. See
        https://developer.gnome.org/gdk-pixbuf/2.36/gdk-pixbuf-Image-Data-in-Memory.html
        */

        gtk_main_iteration_do(FALSE);
        /* Explicitly calling each iteration of the GTK main loop allows LVGL to sync frame
        buffer updates with GTK. It is perhaps also possible to just call gtk_main(), but not
        sure how sync will work then
        */
        usleep(1*1000);
    }
}

static gboolean mouse_pressed(GtkWidget *widget, GdkEventButton *event,
    gpointer user_data)
{
    mouse_btn = LV_INDEV_STATE_PR;
    // Important, if this function returns TRUE the window cannot be moved around inside the browser
    // when using broadway
    return FALSE;
}


static gboolean mouse_released(GtkWidget *widget, GdkEventButton *event,
    gpointer user_data)
{
    mouse_btn = LV_INDEV_STATE_REL;
    // Important, if this function returns TRUE the window cannot be moved around inside the browser
    // when using broadway
    return FALSE;
}

/*****************************************************************************/

static gboolean mouse_motion(GtkWidget *widget, GdkEventMotion *event,
    gpointer user_data)
{
    mouse_x = event->x;
    mouse_y = event->y;
    // Important, if this function returns TRUE the window cannot be moved around inside the browser
    // when using broadway
    return FALSE;
}


static gboolean keyboard_press(GtkWidget *widget, GdkEventKey *event,
    gpointer user_data)
{

    uint32_t ascii_key = event->keyval;
    /*Remap some key to LV_KEY_... to manage groups*/
    switch(event->keyval) {
        case GDK_KEY_rightarrow:
        case GDK_KEY_Right:
            ascii_key =  LV_KEY_RIGHT;
            break;

        case GDK_KEY_leftarrow:
        case GDK_KEY_Left:
            ascii_key =  LV_KEY_LEFT;
            break;

        case GDK_KEY_uparrow:
        case GDK_KEY_Up:
            ascii_key =  LV_KEY_UP;
            break;

        case GDK_KEY_downarrow:
        case GDK_KEY_Down:
            ascii_key =  LV_KEY_DOWN;
            break;

        case GDK_KEY_Escape:
            ascii_key =  LV_KEY_ESC;
            break;

        case GDK_KEY_BackSpace:
            ascii_key =  LV_KEY_BACKSPACE;
            break;

        case GDK_KEY_Delete:
            ascii_key = LV_KEY_DEL;
            break;

        case GDK_KEY_Tab:
            ascii_key = LV_KEY_NEXT;
            break;

        case GDK_KEY_KP_Enter:
        case GDK_KEY_Return:
        case '\r':
            ascii_key = LV_KEY_ENTER;
            break;

        default:
            break;

    }

     last_key = ascii_key;
     last_key_state = LV_INDEV_STATE_PR;
     // For other codes refer to https://developer.gnome.org/gdk3/stable/gdk3-Event-Structures.html#GdkEventKey

     return TRUE;
}

static gboolean keyboard_release(GtkWidget *widget, GdkEventKey *event,
    gpointer user_data)
{
     last_key = 0;
     last_key_state = LV_INDEV_STATE_REL;
     // For other codes refer to https://developer.gnome.org/gdk3/stable/gdk3-Event-Structures.html#GdkEventKey

     return TRUE;
}

static void quit_handler(void)
{
    exit(0);
    run_gtk = FALSE;
}
#endif /*USE_GTK*/

