#pragma once
#define PIN_NUM_BK_LIGHT (5)
#define PIN_NUM_SCLK (18)
#define PIN_NUM_MOSI (23)
#define PIN_NUM_DC (2)
#define PIN_NUM_LCD_RST (4)
#define LCD_PIXEL_CLOCK_HZ (20*1000*1000)
#define LCD_CMD_BITS (8)
#define LCD_PARAM_BITS (8)

#define LCD_WIDTH (240)
#define LCD_HEIGH (240)
#ifdef __cplusplus
extern "C"{
#endif
void bk_init();
void lcd_init();
void lvgl_init();

#ifdef __cplusplus
}
#endif

