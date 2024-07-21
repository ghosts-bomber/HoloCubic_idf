#include "menu.h"
#include "lvgl.h"
LV_FONT_DECLARE(lv_font_montserrat_24);
static lv_style_t app_name_style;
void menu_init(void)
{
    lv_obj_t* app_scr = lv_scr_act();
    lv_obj_set_style_bg_color(app_scr, lv_color_hex(0x003a57), LV_PART_MAIN);

    lv_obj_t* label = lv_label_create(app_scr);
    lv_label_set_text(label, "Hello world");
    lv_obj_set_style_text_color(app_scr, lv_color_hex(0xffffff), LV_PART_MAIN);
    
    lv_style_init(&app_name_style);
    lv_style_set_text_opa(&app_name_style, LV_OPA_COVER);
    lv_style_set_text_color(&app_name_style, lv_color_white());
    lv_style_set_text_font(&app_name_style, &lv_font_montserrat_24);
    lv_obj_add_style(label, &app_name_style, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    
}
