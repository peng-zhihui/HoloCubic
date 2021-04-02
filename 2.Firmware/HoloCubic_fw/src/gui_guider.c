#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"


void setup_ui(lv_ui* ui)
{
	setup_scr_home(ui);
	lv_scr_load(ui->home);

	setup_scr_scenes(ui);
	lv_scr_load(ui->scenes);
}
