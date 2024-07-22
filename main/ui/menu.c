#include "menu.h"
#include "core/lv_disp.h"
#include "core/lv_obj.h"
#include "core/lv_obj_pos.h"
#include "core/lv_obj_style.h"
#include "core/lv_obj_tree.h"
#include "lv_api_map.h"
#include "lvgl.h"
#include "misc/lv_anim.h"
#include "misc/lv_area.h"
#include "misc/lv_style.h"
#include "widgets/lv_img.h"
#include <stdint.h>
#include <esp_log.h>
static const char *TAG = "menu:";
LV_FONT_DECLARE(lv_font_montserrat_24);
static lv_obj_t *app_scr = NULL;
static lv_obj_t *app_scr_t = NULL;
static lv_obj_t *pre_app_image = NULL;
static lv_obj_t *pre_app_name = NULL;
static lv_obj_t *now_app_image = NULL;
static lv_obj_t *now_app_name = NULL;
const void *pre_img_path = NULL;

static lv_style_t default_style;
static lv_style_t app_name_style;
void menu_init(void) {
  if (app_scr != NULL) {
    lv_obj_clean(app_scr);
    app_scr = NULL;
  }
  lv_style_init(&default_style);
  lv_style_set_bg_color(&default_style, lv_color_hex(0x000000));
  lv_style_set_radius(&default_style, 0);

  lv_style_set_border_width(&default_style, 0);

  lv_style_init(&app_name_style);
  lv_style_set_text_opa(&app_name_style, LV_OPA_COVER);
  lv_style_set_text_color(&app_name_style, lv_color_white());
  lv_style_set_text_font(&app_name_style, &lv_font_montserrat_24);

  app_scr = lv_obj_create(NULL);
  lv_obj_add_style(app_scr, &default_style, LV_STATE_DEFAULT);
  // hide scroll bar
  lv_obj_set_style_bg_opa(app_scr, LV_OPA_0,
                          LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

  app_scr_t = lv_obj_create(app_scr);
  lv_obj_add_style(app_scr_t, &default_style, LV_STATE_DEFAULT);
  lv_obj_set_size(app_scr_t, 240, 240);
  lv_obj_align(app_scr_t, LV_ALIGN_CENTER, 0, 0);
  lv_scr_load(app_scr_t);
  
}

void display_app_scr_init(const void *src_img_path, const char *app_name) {
  lv_obj_t *act_obj = lv_scr_act();
  if (act_obj == app_scr) {
    lv_scr_load_anim(app_scr, LV_SCR_LOAD_ANIM_NONE, 300, 300, false);
    return;
  }
  lv_obj_clean(act_obj);
  pre_app_image = lv_img_create(act_obj);
  pre_img_path = src_img_path;
  lv_img_set_src(pre_app_image, src_img_path);
  lv_obj_align(pre_app_image, LV_ALIGN_CENTER, 0, -20);

  pre_app_name = lv_label_create(act_obj);
  lv_obj_add_style(pre_app_name, &app_name_style, LV_STATE_DEFAULT);
  lv_label_set_text(pre_app_name, app_name);
  ESP_LOGI(TAG, "add label");
  lv_obj_align_to(pre_app_name, pre_app_image, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_scr_load_anim(app_scr, LV_SCR_LOAD_ANIM_NONE, 300, 300, false);
}

void app_control_display_scr(const void *src_img, const char *app_name,
                             lv_scr_load_anim_t anim_type, bool force) {
  if (true == force) {
    display_app_scr_init(src_img, app_name);
    return;
  }
  if (src_img == pre_img_path) {
    return;
  }
  pre_img_path = src_img;
  int now_start_x;
  int now_end_x;
  int old_start_x;
  int old_end_x;
  if (LV_SCR_LOAD_ANIM_MOVE_LEFT == anim_type) {
    // 120为半个屏幕大小 应用图标规定是128，一半刚好是64
    now_start_x = -120 - 64;
    now_end_x = 0;
    old_start_x = 0;
    old_end_x = 120 + 64;
  } else {
    // 120为半个屏幕大小 应用图标规定是128，一半刚好是64
    now_start_x = 120 + 64;
    now_end_x = 0;
    old_start_x = 0;
    old_end_x = -120 - 64;
  }

  now_app_image = lv_img_create(app_scr);
  lv_img_set_src(now_app_image, src_img);
  lv_obj_align(now_app_image, LV_ALIGN_CENTER, 0, -20);
  now_app_name = lv_label_create(app_scr);
  lv_obj_add_style(now_app_name, &app_name_style, LV_STATE_DEFAULT);
  lv_label_set_text(now_app_name, app_name);
  lv_obj_del(pre_app_name);
  pre_app_name = now_app_name;
  lv_obj_align_to(now_app_name, now_app_image, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  static lv_anim_t now_app;
  lv_anim_init(&now_app);
  lv_anim_set_exec_cb(&now_app, (lv_anim_exec_xcb_t)lv_obj_set_x);
  lv_anim_set_var(&now_app, now_app_image);
  lv_anim_set_values(&now_app, now_start_x, now_end_x);
  uint32_t duration = lv_anim_speed_to_time(400, now_start_x, now_end_x);
  lv_anim_set_time(&now_app, duration);
  lv_anim_set_path_cb(&now_app, lv_anim_path_linear);

  static lv_anim_t pre_app;
  lv_anim_init(&pre_app);
  lv_anim_set_exec_cb(&pre_app, (lv_anim_exec_xcb_t)lv_obj_set_x);
  lv_anim_set_var(&pre_app, pre_app_image);
  lv_anim_set_values(&pre_app, old_start_x, old_end_x);
  duration = lv_anim_speed_to_time(400, old_start_x, old_end_x); // 计算时间
  lv_anim_set_time(&pre_app, duration);
  lv_anim_set_path_cb(&pre_app, lv_anim_path_linear); // 设置一个动画的路径
  lv_anim_start(&now_app);
  lv_anim_start(&pre_app);

  while (lv_anim_count_running()) {
    // lv_task_handler();
  }
  // lv_task_handler();

  lv_obj_del(pre_app_image);
  pre_app_image = now_app_image;
}
