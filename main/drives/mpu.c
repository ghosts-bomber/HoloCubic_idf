#include "mpu.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "mpu6050.h"
static const char *TAG = "mpu6050:";

static mpu6050_handle_t mpu6050 = NULL;

static void i2c_bus_init(void) {
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
  conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

  esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &conf);
  ESP_ERROR_CHECK(ret);

  ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
  ESP_ERROR_CHECK(ret);
}

void mpu_init() {
  esp_err_t ret;

  i2c_bus_init();
  mpu6050 = mpu6050_create(I2C_MASTER_NUM, MPU6050_I2C_ADDRESS);
  ret = mpu6050_config(mpu6050, ACCE_FS_4G, GYRO_FS_500DPS);
  ESP_ERROR_CHECK(ret);

  ret = mpu6050_wake_up(mpu6050);
  ESP_ERROR_CHECK(ret);
}

void mpu_read_task(void *param) {
  esp_err_t ret;
  uint8_t mpu6050_deviceid;
  // mpu6050_raw_acce_value_t r_acce;
  // mpu6050_raw_gyro_value_t r_gyro;
  mpu6050_acce_value_t acce;
  mpu6050_gyro_value_t gyro;
  complimentary_angle_t angle;

  ret = mpu6050_get_deviceid(mpu6050, &mpu6050_deviceid);
  ESP_ERROR_CHECK(ret);

  while (true) {
    // ret = mpu6050_get_raw_acce(mpu6050, &r_acce);
    // ESP_ERROR_CHECK(ret);
    //
    // ret = mpu6050_get_raw_gyro(mpu6050, &r_gyro);
    // ESP_ERROR_CHECK(ret);
    ret = mpu6050_get_acce(mpu6050, &acce);
    ESP_ERROR_CHECK(ret);

    ret = mpu6050_get_gyro(mpu6050, &gyro);
    ESP_ERROR_CHECK(ret);
    mpu6050_complimentory_filter(mpu6050, &acce, &gyro, &angle);
    //ESP_LOGI(TAG,"pitch:%f roll:%f\n",angle.pitch,angle.roll);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  mpu6050_delete(mpu6050);
  ret = i2c_driver_delete(I2C_MASTER_NUM);
  ESP_ERROR_CHECK(ret);
}

