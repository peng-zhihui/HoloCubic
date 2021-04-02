/*
 * Copyright 2021 NXP
 * SPDX-License-Identifier: MIT
 */

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"


void setup_scr_home(lv_ui* ui)
{
	//Write codes home
	ui->home = lv_obj_create(NULL, NULL);

	//Write codes home_cpicker0
	ui->home_cpicker0 = lv_cpicker_create(ui->home, NULL);

	//Write style LV_CPICKER_PART_MAIN for home_cpicker0
	static lv_style_t style_home_cpicker0_main;
	lv_style_init(&style_home_cpicker0_main);

	//Write style state: LV_STATE_DEFAULT for style_home_cpicker0_main
	lv_style_set_pad_inner(&style_home_cpicker0_main, LV_STATE_DEFAULT, 10);
	lv_style_set_scale_width(&style_home_cpicker0_main, LV_STATE_DEFAULT, 10);
	lv_obj_add_style(ui->home_cpicker0, LV_CPICKER_PART_MAIN, &style_home_cpicker0_main);
	lv_obj_set_pos(ui->home_cpicker0, 15, 16);
	lv_obj_set_size(ui->home_cpicker0, 200, 200);
	lv_cpicker_set_type(ui->home_cpicker0, LV_CPICKER_TYPE_DISC);
}