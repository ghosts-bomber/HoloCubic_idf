#pragma once

struct APP_OBJ {
  const char *app_name;
  const void *app_image;
  const char *app_info;
  int (*app_init)();
  int (*exit_callback)(void *param);
};
