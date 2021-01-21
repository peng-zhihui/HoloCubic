#include "../../../lv_examples.h"
#if LV_USE_KEYBOARD

void lv_ex_keyboard_1(void)
{
    /*Create a keyboard and apply the styles*/
    lv_obj_t *kb = lv_keyboard_create(lv_scr_act(), NULL);
    lv_keyboard_set_cursor_manage(kb, true);

    /*Create a text area. The keyboard will write here*/
    lv_obj_t *ta = lv_textarea_create(lv_scr_act(), NULL);
    lv_obj_align(ta, NULL, LV_ALIGN_IN_TOP_MID, 0, LV_DPI / 16);
    lv_textarea_set_text(ta, "");
    lv_coord_t max_h = LV_VER_RES / 2 - LV_DPI / 8;
    if(lv_obj_get_height(ta) > max_h) lv_obj_set_height(ta, max_h);

    /*Assign the text area to the keyboard*/
    lv_keyboard_set_textarea(kb, ta);
}
#endif
