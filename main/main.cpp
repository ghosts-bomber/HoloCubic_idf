
#include "nvs.h"
#include <esp_log.h>
#include <esp_err.h>
#include <nvs_flash.h>
#include "display.h"
static const char *TAG = "main";

static void init_nvs() {
  esp_err_t ret = nvs_flash_init();
  if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret==ESP_ERR_NVS_NEW_VERSION_FOUND){
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

}
extern "C" void app_main(void) { 
  esp_log_level_set(TAG, ESP_LOG_INFO); 
  init_nvs(); 
  bk_init();
  lcd_init();
  lvgl_init();

}
  
