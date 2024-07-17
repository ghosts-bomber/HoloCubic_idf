#pragma once
#include "app_interface.h"


class AppContorller{
  public:
    AppContorller();
    ~AppContorller();
    void init();
    int app_install(APP_OBJ *app);
    int app_uninstall(const APP_OBJ *app);
    void enter_app(const APP_OBJ* app);
    void app_exit();

};
