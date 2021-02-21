/*********************
 *      INCLUDES
 *********************/
#include "lv_cubic_gui.h"
#include "images.h"
#include "lv_port_indev.h"


lv_obj_t* scr1;
lv_obj_t* scr2;
lv_obj_t* scr3;
lv_obj_t* label;
lv_obj_t* labe2;
lv_obj_t* labe3;
lv_font_declare(lv_font_montserrat_20);
lv_font_declare(lv_font_montserrat_24);
lv_font_declare(lv_font_montserrat_40);

static lv_group_t* g;
static lv_obj_t* tv;
static lv_obj_t* t1;
static lv_obj_t* t2;
static lv_obj_t* t3;


void lv_holo_cubic_gui(const char* weachern, const char* fens, const char* cityname2)
{
	static lv_style_t default_style;
	static lv_style_t label_style;
	static lv_style_t label_style2;
	static lv_style_t label_style3;
	static lv_style_t label_style4;
	lv_style_init(&default_style);
	lv_style_set_bg_color(&default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
	lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));
	lv_obj_add_style(lv_scr_act(), LV_BTN_PART_MAIN, &default_style);

	scr1 = lv_scr_act();//在第一页创建
	lv_obj_t* img1 = lv_img_create(scr1, NULL);
	lv_img_set_src(img1, &tianqi);
	lv_obj_align(img1, NULL, LV_ALIGN_OUT_TOP_MID, 0, 140);

	lv_style_init(&label_style);
	lv_style_set_text_opa(&label_style, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_text_color(&label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&label_style, LV_STATE_DEFAULT, &lv_font_montserrat_24);
	lv_obj_t* label = lv_label_create(scr1, NULL);
	lv_obj_add_style(label, LV_LABEL_PART_MAIN, &label_style);
	lv_label_set_text(label, cityname2);
	lv_obj_align(label, NULL, LV_ALIGN_OUT_BOTTOM_LEFT, 25, -65);


	lv_style_init(&label_style2);
	lv_style_set_text_opa(&label_style2, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_text_color(&label_style2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&label_style2, LV_STATE_DEFAULT, &lv_font_montserrat_40);
	lv_obj_t* labe2 = lv_label_create(scr1, NULL);
	lv_obj_add_style(labe2, LV_LABEL_PART_MAIN, &label_style2);
	lv_label_set_text(labe2, weachern);
	lv_obj_align(labe2, label, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

	lv_style_init(&label_style3);
	lv_style_set_text_opa(&label_style3, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_text_color(&label_style3, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&label_style3, LV_STATE_DEFAULT, &lv_font_montserrat_24);
	lv_obj_t* labe3 = lv_label_create(scr1, NULL);
	lv_obj_add_style(labe3, LV_LABEL_PART_MAIN, &label_style3);
	lv_label_set_text(labe3, "°C");
	lv_obj_align(labe3, labe2, LV_ALIGN_OUT_RIGHT_MID, 10, 0);


	scr2 = lv_obj_create(NULL, NULL);//在第二页创建
	lv_obj_add_style(scr2, LV_BTN_PART_MAIN, &default_style);
	lv_obj_t* img4 = lv_img_create(scr2, NULL);
	lv_img_set_src(img4, &logo);
	lv_obj_align(img4, NULL, LV_ALIGN_OUT_TOP_MID, 0, 110);
	lv_obj_t* img3 = lv_img_create(scr2, NULL);
	lv_img_set_src(img3, &huoj);
	lv_obj_align(img3, NULL, LV_ALIGN_OUT_BOTTOM_RIGHT, -15, -100);


	lv_style_init(&label_style);
	lv_style_set_text_opa(&label_style, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_text_color(&label_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&label_style, LV_STATE_DEFAULT, &lv_font_montserrat_24);
	lv_obj_t* labe4 = lv_label_create(scr2, NULL);
	lv_obj_add_style(labe4, LV_LABEL_PART_MAIN, &label_style);
	lv_label_set_text(labe4, "- CWEIB -");
	lv_obj_align(labe4, NULL, LV_ALIGN_OUT_BOTTOM_LEFT, 30, -95);


	lv_style_init(&label_style2);
	lv_style_set_text_opa(&label_style2, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_text_color(&label_style2, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&label_style2, LV_STATE_DEFAULT, &lv_font_montserrat_40);
	lv_obj_t* labe5 = lv_label_create(scr2, NULL);
	lv_obj_add_style(labe5, LV_LABEL_PART_MAIN, &label_style2);
	lv_label_set_text(labe5, fens);
	lv_obj_align(labe5, labe4, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);


	scr3 = lv_obj_create(NULL, NULL);
	lv_obj_add_style(scr3, LV_BTN_PART_MAIN, &default_style);
	lv_style_set_text_opa(&label_style4, LV_STATE_DEFAULT, LV_OPA_COVER);
	lv_style_set_text_color(&label_style4, LV_STATE_DEFAULT, LV_COLOR_WHITE);
	lv_style_set_text_font(&label_style4, LV_STATE_DEFAULT, &lv_font_montserrat_20);

	lv_obj_t* labe6 = lv_label_create(scr3, NULL);
	lv_obj_add_style(labe6, LV_LABEL_PART_MAIN, &label_style4);
	lv_label_set_text_fmt(labe6, "CPU Temp: %d °C", 0);
	lv_obj_set_pos(labe6, 2, 30);

	lv_obj_t* labe7 = lv_label_create(scr3, NULL);
	lv_obj_add_style(labe7, LV_LABEL_PART_MAIN, &label_style4);
	lv_label_set_text_fmt(labe7, "CPU Usage: %d\%", 0);
	lv_obj_set_pos(labe7, 2, 60);

	lv_obj_t* labe8 = lv_label_create(scr3, NULL);
	lv_obj_add_style(labe8, LV_LABEL_PART_MAIN, &label_style4);
	lv_label_set_text_fmt(labe8, "Mem Usage: %dMB", 0);
	lv_obj_set_pos(labe8, 2, 90);

	lv_obj_t* labe9 = lv_label_create(scr3, NULL);
	lv_obj_add_style(labe9, LV_LABEL_PART_MAIN, &label_style4);
	lv_label_set_text_fmt(labe9, "Net Upload: %dKB/s", 0);
	lv_obj_set_pos(labe9, 2, 120);

	lv_obj_t* labe10 = lv_label_create(scr3, NULL);
	lv_obj_add_style(labe10, LV_LABEL_PART_MAIN, &label_style4);
	lv_label_set_text_fmt(labe10, "Net Download: %dKB/s", 0);
	lv_obj_set_pos(labe10, 2, 150);
}