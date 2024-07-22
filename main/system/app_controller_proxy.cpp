#include "app_controller_proxy.h"
#include "menu.h"

AppControllerProxy::AppControllerProxy(AppContorller *controller)
    : controller_(controller) {}

AppControllerProxy::~AppControllerProxy() {}

void AppControllerProxy::init() {
  controller_->init();
  menu_init();
  

}
