/**
 * @file libinput.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "libinput_drv.h"
#if USE_LIBINPUT != 0

#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <poll.h>
#include <libinput.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static int open_restricted(const char *path, int flags, void *user_data);
static void close_restricted(int fd, void *user_data);

/**********************
 *  STATIC VARIABLES
 **********************/
static int libinput_fd;
static int libinput_button;
static const int timeout = 0; // do not block
static const nfds_t nfds = 1;
static struct pollfd fds[1];
static lv_point_t most_recent_touch_point = { .x = 0, .y = 0};

static struct libinput *libinput_context;
static struct libinput_device *libinput_device;
const static struct libinput_interface interface = {
  .open_restricted = open_restricted,
  .close_restricted = close_restricted,
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * reconfigure the device file for libinput
 * @param dev_name set the libinput device filename
 * @return true: the device file set complete
 *         false: the device file doesn't exist current system
 */
bool libinput_set_file(char* dev_name)
{
  // This check *should* not be necessary, yet applications crashes even on NULL handles.
  // citing libinput.h:libinput_path_remove_device:
  // > If no matching device exists, this function does nothing.
  if (libinput_device) {
    libinput_device = libinput_device_unref(libinput_device);
    libinput_path_remove_device(libinput_device);
  }

  libinput_device = libinput_path_add_device(libinput_context, dev_name);
  if(!libinput_device) {
    perror("unable to add device to libinput context:");
    return false;
  }
  libinput_device = libinput_device_ref(libinput_device);
  if(!libinput_device) {
    perror("unable to reference device within libinput context:");
    return false;
  }

  libinput_button = LV_INDEV_STATE_REL;

  return true;
}

/**
 * Initialize the libinput interface
 */
void libinput_init(void)
{
  libinput_device = NULL;
  libinput_context = libinput_path_create_context(&interface, NULL);
  if(!libinput_set_file(LIBINPUT_NAME)) {
      perror("unable to add device \"" LIBINPUT_NAME "\" to libinput context:");
      return;
  }
  libinput_fd = libinput_get_fd(libinput_context);

  /* prepare poll */
  fds[0].fd = libinput_fd;
  fds[0].events = POLLIN;
  fds[0].revents = 0;
}

/**
 * Get the current position and state of the libinput
 * @param indev_drv driver object itself
 * @param data store the libinput data here
 * @return false: because the points are not buffered, so no more data to be read
 */
bool libinput_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
  struct libinput_event *event;
  struct libinput_event_touch *touch_event = NULL;
  int rc = 0;

  rc = poll(fds, nfds, timeout);
  switch (rc){
    case -1:
      perror(NULL);
    case 0:
      goto report_most_recent_state;
    default:
      break;
  }
  libinput_dispatch(libinput_context);
  while((event = libinput_get_event(libinput_context)) != NULL) {
    enum libinput_event_type type = libinput_event_get_type(event);
    switch (type) {
      case LIBINPUT_EVENT_TOUCH_MOTION:
      case LIBINPUT_EVENT_TOUCH_DOWN:
        touch_event = libinput_event_get_touch_event(event);
        most_recent_touch_point.x = libinput_event_touch_get_x_transformed(touch_event, LV_HOR_RES);
        most_recent_touch_point.y = libinput_event_touch_get_y_transformed(touch_event, LV_VER_RES);
        libinput_button = LV_INDEV_STATE_PR;
        break;
      case LIBINPUT_EVENT_TOUCH_UP:
        libinput_button = LV_INDEV_STATE_REL;
        break;
      default:
        break;
    }
    libinput_event_destroy(event);
  }
report_most_recent_state:
  data->point.x = most_recent_touch_point.x;
  data->point.y = most_recent_touch_point.y;
  data->state = libinput_button;

  return false;
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

static int open_restricted(const char *path, int flags, void *user_data)
{
  int fd = open(path, flags);
  return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data)
{
  close(fd);
}

#endif
