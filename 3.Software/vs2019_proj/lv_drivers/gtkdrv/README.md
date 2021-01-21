# Add GTK under Linux in Eclipse

## Install GDK

```
sudo apt-get install libgtk-3-dev
sudo apt-get install libglib2.0-dev
```

## Add GTK include paths and libraries

In "Project properties > C/C++ Build > Settings" set the followings:

- "Cross GCC Compiler > Command line pattern"
  - Add ` ${gtk+-cflags}` to the end (add a space between the last command and this)
  
- "Cross GCC Compiler > Includes"
  - /usr/include/glib-2.0
  - /usr/include/gtk-3.0
  - /usr/include/pango-1.0
  - /usr/include/cairo
  - /usr/include/gdk-pixbuf-2.0
  - /usr/include/atk-1.0
		
- "Cross GCC Linker > Command line pattern"
  - Add ` ${gtk+-libs}` to the end (add a space between the last command and this)
 
 
- "Cross GCC Linker > Libraries"
  - Add `pthread`
 
 
- In "C/C++ Build > Build variables"
  - Configuration: [All Configuration]

  - Add
    - Variable name: `gtk+-cflags`
      - Type: `String`
      - Value: `pkg-config --cflags gtk+-3.0`
    - Variable name: `gtk+-libs`
      - Type: `String`
      - Value: `pkg-config --libs gtk+-3.0`


## Init GDK in LVGL

1. In `main.c` `#incude "lv_drivers/gdkdrv/gdkdrv.h"`
2. Enable the GTK driver in `lv_drv_conf.h` with `USE_GTK 1`
3. After `lv_init()` call `gdkdrv_init()`; 
4. Add a display:
```c
  static lv_disp_buf_t disp_buf1;
  static lv_color_t buf1_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];
  lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);

  /*Create a display*/
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.buffer = &disp_buf1;
  disp_drv.flush_cb = gtkdrv_flush_cb;
```
5. Add mouse:
```c
  lv_indev_drv_t indev_drv_mouse;
  lv_indev_drv_init(&indev_drv_mouse);
  indev_drv_mouse.type = LV_INDEV_TYPE_POINTER;
```
6. Akk keyboard:
```c
  lv_indev_drv_t indev_drv_kb;
  lv_indev_drv_init(&indev_drv_kb);
  indev_drv_kb.type = LV_INDEV_TYPE_KEYPAD;
  indev_drv_kb.read_cb = lv_keyboard_read_cb;
  lv_indev_drv_register(&indev_drv_kb);
```
7. Configure tick in `lv_conf.h`
```c
#define LV_TICK_CUSTOM     1
#if LV_TICK_CUSTOM == 1
#define LV_TICK_CUSTOM_INCLUDE  "lv_drivers/gtkdrv/gtkdrv.h"       /*Header for the sys time function*/
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (gtkdrv_tick_get())     /*Expression evaluating to current systime in ms*/
#endif   /*LV_TICK_CUSTOM*/
```
8. Be sure `LV_COLOR_DEPTH` is `32` in `lv_conf.h`

## Run in a window
Build and Run to "normally" run the UI in a window  

## Run in browser
With the help of `Broadway` the UI can be easily shown via a browser. 

1. Open Terminal and start *Broadway* with `broadwayd :5`. Leave the terminal running.
2. Navigate to where eclipse created the binary executable (my_project/Debug) and open a terminal in that folder. 
In this terminal run `GDK_BACKEND=broadway BROADWAY_DISPLAY=:5 ./my_executable` (replace *my_executable* wih name of your executable)
3. Open a web browser and go to `http://localhost:8085/`

![LVGL with GTK/GDK Broadway backend](https://github.com/lvgl/lv_drivers/blob/master/gtkdrv/broadway.png?raw=true)
