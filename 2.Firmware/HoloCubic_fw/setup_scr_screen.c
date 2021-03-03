#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"


void setup_scr_screen(lv_ui* ui)
{

	//Write codes screen
	ui->screen = lv_obj_create(NULL, NULL);

	//Write codes screen_img0
	ui->screen_img0 = lv_img_create(ui->screen, NULL);

	//Write style LV_IMG_PART_MAIN for screen_img0
	static lv_style_t style_screen_img0_main;
	lv_style_init(&style_screen_img0_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_img0_main
	lv_style_set_image_recolor(&style_screen_img0_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_image_recolor_opa(&style_screen_img0_main, LV_STATE_DEFAULT, 0);
	lv_style_set_image_opa(&style_screen_img0_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_img0, LV_IMG_PART_MAIN, &style_screen_img0_main);
	lv_obj_set_pos(ui->screen_img0, 0, 0);
	lv_obj_set_size(ui->screen_img0, 240, 240);
	lv_obj_set_click(ui->screen_img0, true);
	lv_img_set_src(ui->screen_img0, "S:/logo.bin");
	lv_img_set_pivot(ui->screen_img0, 0, 0);
	lv_img_set_angle(ui->screen_img0, 0);

	//Write codes screen_btn1
	ui->screen_btn1 = lv_btn_create(ui->screen, NULL);

	//Write style LV_BTN_PART_MAIN for screen_btn1
	static lv_style_t style_screen_btn1_main;
	lv_style_init(&style_screen_btn1_main);

	//Write style state: LV_STATE_DEFAULT for style_screen_btn1_main
	lv_style_set_radius(&style_screen_btn1_main, LV_STATE_DEFAULT, 50);
	lv_style_set_bg_color(&style_screen_btn1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_color(&style_screen_btn1_main, LV_STATE_DEFAULT, lv_color_make(0xff, 0xff, 0xff));
	lv_style_set_bg_grad_dir(&style_screen_btn1_main, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
	lv_style_set_bg_opa(&style_screen_btn1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_border_color(&style_screen_btn1_main, LV_STATE_DEFAULT, lv_color_make(0x01, 0xa2, 0xb1));
	lv_style_set_border_width(&style_screen_btn1_main, LV_STATE_DEFAULT, 2);
	lv_style_set_border_opa(&style_screen_btn1_main, LV_STATE_DEFAULT, 255);
	lv_style_set_outline_color(&style_screen_btn1_main, LV_STATE_DEFAULT, lv_color_make(0xd4, 0xd7, 0xd9));
	lv_style_set_outline_opa(&style_screen_btn1_main, LV_STATE_DEFAULT, 255);
	lv_obj_add_style(ui->screen_btn1, LV_BTN_PART_MAIN, &style_screen_btn1_main);
	lv_obj_set_pos(ui->screen_btn1, 81, 159);
	lv_obj_set_size(ui->screen_btn1, 100, 50);
	ui->screen_btn1_label = lv_label_create(ui->screen_btn1, NULL);
	lv_label_set_text(ui->screen_btn1_label, "default");
	lv_obj_set_style_local_text_color(ui->screen_btn1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_make(0x00, 0x00, 0x00));
	lv_obj_set_style_local_text_font(ui->screen_btn1_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_simsun_12);
}