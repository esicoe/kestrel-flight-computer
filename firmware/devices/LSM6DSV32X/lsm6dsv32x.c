#include "lsm6dsv32x.h"

register_setting_t lsm6dsv32xConfig[] = {
    {}
}

bool LSM6DSV32X_Initialize() {
    bool status = true;
    uint8_t output;
    uint8_t input;

    status &= I2C0_Read(ADDRESS_IMU_I2C, ADDRESS_IMU_WHO_AM_I, &output, 1);
    status &= (output == VALUE_IMU_WHO_AM_I);

    input = VALUE_IMU_SW_RESET;
    status &= I2C0_Write(ADDRESS_IMU_I2C, ADDRESS_IMU_CTRL3, &input, 1);

    for (uint8_t i = 0; i < 10; i++) {
        timeout_ms(50);

        status &= I2C0_Read(ADDRESS_IMU_I2C, ADDRESS_IMU_CTRL3, &output, 1);
    }

    status &= !(output << 7);

    for (uint8_t i = 0; i < (sizeof(lsm6dsv32xConfig) / sizeof(lsm6dsv32xConfig[0])); i++) {
        
    }
}