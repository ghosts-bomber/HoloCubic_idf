#include "app_controller.h"

AppContorller::AppContorller() {}
AppContorller::~AppContorller() {}
void AppContorller::init() {}
int AppContorller::app_install(APP_OBJ *app) { return 0; }
int AppContorller::app_uninstall(const APP_OBJ *app) { return 0; }

void AppContorller::enter_app(const APP_OBJ *app) {}
void AppContorller::app_exit() {}
