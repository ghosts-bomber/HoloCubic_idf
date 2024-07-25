#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
// lvgl 操作的锁
extern SemaphoreHandle_t lvgl_mutex;
// LVGL操作的安全宏（避免脏数据）
#define AIO_LVGL_OPERATE_LOCK(CODE)                          \
    if (pdTRUE == xSemaphoreTake(lvgl_mutex, portMAX_DELAY)) \
    {                                                        \
        CODE;                                                \
        xSemaphoreGive(lvgl_mutex);                          \
    }

typedef enum 
{
    TURN_RIGHT = 0,
    RETURN,
    TURN_LEFT,
    UP,
    DOWN,
    GO_FORWORD,
    SHAKE,
    UNKNOWN
}ACTIVE_TYPE;

// 方向类型
typedef enum 
{
    NORMAL_DIR_TYPE = 0,
    X_DIR_TYPE = 0x01,
    Y_DIR_TYPE = 0x02,
    Z_DIR_TYPE = 0x04,
    XY_DIR_TYPE = 0x08
}MPU_DIR_TYPE;

typedef struct 
{
    int16_t x_gyro_offset;
    int16_t y_gyro_offset;
    int16_t z_gyro_offset;

    int16_t x_accel_offset;
    int16_t y_accel_offset;
    int16_t z_accel_offset;
}SysMpuConfig;

typedef struct 
{
    volatile ACTIVE_TYPE active;
    uint8_t isValid;
    uint8_t long_time;
    int16_t v_ax; // v表示虚拟参数（用于调整6050的初始方位）
    int16_t v_ay;
    int16_t v_az;
    int16_t v_gx;
    int16_t v_gy;
    int16_t v_gz;
} ImuAction;
