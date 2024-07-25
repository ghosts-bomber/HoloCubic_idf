#pragma once
#include "app_controller.h"
class AppControllerProxy {
public:
  AppControllerProxy(AppController *controller);
  ~AppControllerProxy();

  void init();
private:
  AppController *controller_ = nullptr;
};
