#include "app_controller_proxy.h"
#include "menu.h"
#include "app_conf.h"
#include "esp_log.h"
static const char *TAG = "AppControllerProxy:";

AppControllerProxy::AppControllerProxy(AppController *controller)
    : controller_(controller) {}

AppControllerProxy::~AppControllerProxy() {}

void AppControllerProxy::init() {
  controller_->init();
  controller_->app_install(&game_2048_app);
  menu_init();
  APP_OBJ* obj = controller_->cur_app();
  if(obj){
    ESP_LOGI(TAG, "show  menu app");
    app_control_display_scr(obj->app_image,obj->app_name,LV_SCR_LOAD_ANIM_NONE,true);
  }

}
