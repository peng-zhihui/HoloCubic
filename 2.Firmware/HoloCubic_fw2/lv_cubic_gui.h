#ifndef LV_CUBIC_GUI_H
#define LV_CUBIC_GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

	//extern lv_img_dsc_t screen_buffer;


	extern lv_obj_t* scr1;
	extern lv_obj_t* scr2;
	extern lv_obj_t* scr3;
	extern lv_obj_t* label;
	extern lv_obj_t* labe2;
	extern lv_obj_t* labe3;
	void lv_holo_cubic_gui(const char* weachern, const char* fens, const char* cityname2);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  
