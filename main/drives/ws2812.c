#include "ws2812.h"
#include "esp_err.h"
#include "esp_log.h"
#include "led_strip.h"
static const char *TAG = "ws2812:";

static led_strip_handle_t led_strip;
void ws2812_init() {
  // LED strip general initialization, according to your led board design
  led_strip_config_t strip_config = {
      .strip_gpio_num = LED_STRIP_BLINK_GPIO, // The GPIO that connected to the
                                              // LED strip's data line
      .max_leds = LED_STRIP_LED_NUMBERS, // The number of LEDs in the strip,
      .led_pixel_format =
          LED_PIXEL_FORMAT_GRB,      // Pixel format of your LED strip
      .led_model = LED_MODEL_WS2812, // LED strip model
      .flags.invert_out = false,     // whether to invert the output signal
  };

  // LED strip backend configuration: RMT
  led_strip_rmt_config_t rmt_config = {
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
    .rmt_channel = 0,
#else
    .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to
                                           // different power consumption
    .resolution_hz = LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
    .flags.with_dma =
        false, // DMA feature is available on ESP target like ESP32-S3
#endif
  };

  // LED Strip object handle
  ESP_ERROR_CHECK(
      led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
  ESP_LOGI(TAG, "Created LED strip object with RMT backend");
}

void ws2812_set_rgb(int led_id, int r, int g, int b) {
  ESP_ERROR_CHECK(led_strip_set_pixel(led_strip, led_id, r, g, b));
}
void ws2812_set_finish() { ESP_ERROR_CHECK(led_strip_refresh(led_strip)); }

void ws2812_clear() { ESP_ERROR_CHECK(led_strip_clear(led_strip)); }
