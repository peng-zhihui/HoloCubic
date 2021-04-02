/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"


void setup_scr_scenes(lv_ui* ui)
{

	//Write codes scenes
	ui->scenes = lv_obj_create(NULL, NULL);

	//Write codes scenes_canvas
	ui->scenes_canvas = lv_img_create(ui->scenes, NULL);

	//Write style LV_IMG_PART_MAIN for scenes_canvas
	static lv_style_t style_scenes_canvas_main;
	lv_style_init(&style_scenes_canvas_main);
	lv_style_set_bg_color(&style_scenes_canvas_main, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	lv_style_set_bg_color(&style_scenes_canvas_main, LV_STATE_PRESSED, LV_COLOR_GRAY);
	lv_style_set_bg_color(&style_scenes_canvas_main, LV_STATE_FOCUSED, LV_COLOR_BLACK);

	//Write style state: LV_STATE_DEFAULT for style_scenes_canvas_main
	lv_obj_add_style(ui->scenes, LV_BTN_PART_MAIN, &style_scenes_canvas_main);
	lv_img_set_src(ui->scenes_canvas, "S:/Scenes/Holo3D/frame000.bin");
	lv_obj_align(ui->scenes_canvas, NULL, LV_ALIGN_CENTER, 0, 0);
}