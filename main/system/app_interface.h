#pragma once
#include "control_action_interface.h"
#ifdef __cplusplus
extern "C"{
#endif
typedef struct  {
  const char *app_name;
  const void *app_image;
  const char *app_info;
  int (*app_init)();
  int (*exit_callback)(void *param);
  int (*process_action)(const ControlAction* action);
} APP_OBJ;
#ifdef __cplusplus
}
#endif