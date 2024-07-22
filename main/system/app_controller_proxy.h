#pragma once
#include "app_controller.h"
class AppControllerProxy {
public:
  AppControllerProxy(AppContorller *controller);
  ~AppControllerProxy();

  void init();
private:
  AppContorller *controller_ = nullptr;
};
