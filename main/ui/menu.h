#pragma once

#include "lvgl.h"
#ifdef __cplusplus
extern "C"{
#endif

void menu_init(void);

void app_control_display_scr(const void *src_img, const char *app_name,
                             lv_scr_load_anim_t anim_type, bool force);

#ifdef __cplusplus
}
#endif
