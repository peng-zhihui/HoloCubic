/*********************
 *      INCLUDES
 *********************/
#include "lv_cubic_gui.h"
#include "images.h"
 

lv_obj_t* scr1;
lv_obj_t* scr2;

lv_obj_t* label1;
lv_obj_t* label2;
lv_obj_t* label3;
lv_obj_t* label4;
lv_obj_t* label5;

void lv_holo_cubic_gui(void)
{
	static lv_style_t default_style;
	lv_style_init(&default_style);
	lv_style_set_bg_color(&default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));

	lv_obj_add_style(lv_scr_act(), LV_BTN_PART_MAIN, &default_style);

	scr1 = lv_scr_act();
	lv_obj_t* img1 = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img1, &bilibili);
	lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);

	scr2 = lv_obj_create(NULL, NULL);
	lv_obj_t* img2 = lv_img_create(scr2, NULL);
	lv_img_set_src(img2, &bdgw);
	lv_obj_align(img2, NULL, LV_ALIGN_CENTER, 0, 0);


	//label1 = lv_label_create(scr1, NULL);
	//lv_label_set_text_fmt(label1, "CPU Temp: %d C", 0);
	//lv_obj_set_pos(label1, 20, 30);

	//label2 = lv_label_create(scr1, NULL);
	//lv_label_set_text_fmt(label2, "CPU Usage: %d\%", 0);
	//lv_obj_set_pos(label2, 20, 45);

	//label3 = lv_label_create(scr1, NULL);
	//lv_label_set_text_fmt(label3, "Mem Usage: %dMB", 0);
	//lv_obj_set_pos(label3, 20, 60);

	//label4 = lv_label_create(scr1, NULL);
	//lv_label_set_text_fmt(label4, "Net Upload: %dKB/s", 0);
	//lv_obj_set_pos(label4, 20, 75);

	//label5 = lv_label_create(scr1, NULL);
	//lv_label_set_text_fmt(label5, "Net Download: %dKB/s", 0);
	//lv_obj_set_pos(label5, 20, 90);

 
	
}