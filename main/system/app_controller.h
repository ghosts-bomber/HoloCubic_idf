#pragma once
#include "app_interface.h"
#include "control_action_interface.h"
#define APP_MAX_NUM (32)
class AppContorller
{
public:
  AppContorller();
  ~AppContorller();
  void init();
  int app_install(APP_OBJ *app);
  int app_uninstall(const APP_OBJ *app);
  void enter_app(const APP_OBJ *app);
  void app_exit();
  void process_control(const ControlAction* action);
  APP_OBJ* next_app();
  APP_OBJ* pre_app();
  APP_OBJ* cur_app();
private:
  APP_OBJ * app_list_[APP_MAX_NUM];
  int app_num_ = 0;
  int cur_app_index_ = -1;
  bool menu_ = true;
};
