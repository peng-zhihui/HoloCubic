#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"


void setup_ui(lv_ui* ui)
{
	setup_scr_screen(ui);
	lv_scr_load(ui->screen);
}
