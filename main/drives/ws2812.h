#pragma once

// GPIO assignment
#define LED_STRIP_BLINK_GPIO  27
// Numbers of the LED in the strip
#define LED_STRIP_LED_NUMBERS 1
// 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define LED_STRIP_RMT_RES_HZ  (10 * 1000 * 1000)


#ifdef __cplusplus
extern "C"{
#endif

void ws2812_init();
void ws2812_set_rgb(int led_id,int r,int g,int b);
void ws2812_set_finish();
void ws2812_clear();

#ifdef __cplusplus
}
#endif

