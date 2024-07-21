#pragma once

#define I2C_MASTER_SCL_IO 33      /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 32      /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */

#ifdef __cplusplus
extern "C"{
#endif

void mpu_init();
void mpu_read_task(void *param);

#ifdef __cplusplus
}
#endif

