#include "display.h"
#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_commands.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_types.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "misc/lv_color.h"
#include <stdint.h>
SemaphoreHandle_t lvgl_mutex;
#define LCD_HOST SPI2_HOST
#define LVGL_TICK_PERIOD_MS 5
static const char *TAG = "display:";

static esp_lcd_panel_handle_t panel_handle = NULL;
static lv_disp_draw_buf_t
    disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
static lv_disp_drv_t disp_drv; // contains callback functions

void bk_init() {
  gpio_config_t bk_gpio_config = {.mode = GPIO_MODE_OUTPUT,
                                  .pin_bit_mask = 1ULL << PIN_NUM_BK_LIGHT};
  ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
  gpio_set_level(PIN_NUM_BK_LIGHT, 0);
}

static bool notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io,
                                    esp_lcd_panel_io_event_data_t *edata,
                                    void *user_ctx) {
  lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
  lv_disp_flush_ready(disp_driver);
  return false;
}

void lcd_init() {
  ESP_LOGI(TAG, "Initialize SPI bus");
  spi_bus_config_t buscfg = {.sclk_io_num = PIN_NUM_SCLK,
                             .mosi_io_num = PIN_NUM_MOSI,
                             .miso_io_num = GPIO_NUM_NC,
                             .quadwp_io_num = GPIO_NUM_NC,
                             .quadhd_io_num = GPIO_NUM_NC,
                             .max_transfer_sz =
                                 LCD_HEIGH * 80 * sizeof(uint16_t)};
  ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config = {
      .dc_gpio_num = PIN_NUM_DC,
      .cs_gpio_num = GPIO_NUM_NC,
      .pclk_hz = LCD_PIXEL_CLOCK_HZ,
      .lcd_cmd_bits = LCD_CMD_BITS,
      .lcd_param_bits = LCD_PARAM_BITS,
      .spi_mode = 3,
      .trans_queue_depth = 10,
      .on_color_trans_done = notify_lvgl_flush_ready,
      .user_ctx = &disp_drv,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST,
                                           &io_config, &io_handle));

  ESP_LOGI(TAG, "create esp lcd new panel");

  esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = PIN_NUM_LCD_RST,
      .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
      .bits_per_pixel = 16,
  };
  // Create LCD panel handle for ST7789, with the SPI IO device handle
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle,true));
  ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
}

static void disp_flush(struct _lv_disp_drv_t *drv, const lv_area_t *area,
                       lv_color_t *color_p) {
  esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)drv->user_data;
  int offsetx1 = area->x1;
  int offsetx2 = area->x2;
  int offsety1 = area->y1;
  int offsety2 = area->y2;
  // copy a buffer's content to a specific area of the display
  esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1,
                            offsety2 + 1, color_p);
}

static void increase_lvgl_tick(void *arg) {
  /* Tell LVGL how many milliseconds has elapsed */
  lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

void lvgl_init() {
  lv_init();

  lv_color_t *buf1 =
      (lv_color_t *)heap_caps_malloc(LCD_HEIGH * 20 * sizeof(lv_color_t),
                                     MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
  assert(buf1);
  lv_color_t *buf2 =
      (lv_color_t *)heap_caps_malloc(LCD_HEIGH * 20 * sizeof(lv_color_t),
                                     MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
  assert(buf2);
  // initialize LVGL draw buffers
  lv_disp_draw_buf_init(&disp_buf, buf1, buf2, LCD_HEIGH * 20);
  ESP_LOGI(TAG, "Register display driver to LVGL");

  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = LCD_HEIGH;
  disp_drv.ver_res = LCD_WIDTH;
  disp_drv.flush_cb = disp_flush;
  disp_drv.drv_update_cb = NULL;
  disp_drv.draw_buf = &disp_buf;
  disp_drv.user_data = panel_handle;
  lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

  ESP_LOGI(TAG, "Install LVGL tick timer");
  // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
  const esp_timer_create_args_t lvgl_tick_timer_args = {
      .callback = &increase_lvgl_tick, .name = "lvgl_tick"};
  esp_timer_handle_t lvgl_tick_timer = NULL;
  ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
  ESP_ERROR_CHECK(
      esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));
}

void lvgl_loop_task(void *params){
  while (1) {
    // raise the task priority of LVGL and/or reduce the handler period can
    // improve the performance
    vTaskDelay(pdMS_TO_TICKS(30));
    // The task running lv_timer_handler should have lower priority than that
    // running `lv_tick_inc`
    //AIO_LVGL_OPERATE_LOCK(lv_timer_handler());
    lv_timer_handler();
  }
}

