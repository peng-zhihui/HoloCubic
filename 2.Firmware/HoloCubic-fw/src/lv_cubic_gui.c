/*********************
 *      INCLUDES
 *********************/
#include "lv_cubic_gui.h"
#include "images.h"

lv_obj_t* scr;

void lv_holo_cubic_gui(void)
{
	static lv_style_t default_style;
	lv_style_init(&default_style);
	lv_style_set_bg_color(&default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));

	lv_obj_add_style(lv_scr_act(), LV_BTN_PART_MAIN, &default_style);

	scr = lv_scr_act();
	lv_obj_t* img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &logo);
//	lv_img_set_src(img, "S:/pic.bin");
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
}