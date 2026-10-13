#include "lsm6dsv32x.h"

bool LSM6DSV32X_Initialize() {
    bool status = true;
    uint8_t output;
    uint8_t input;

    status &= I2C0_Read(ADDRESS_IMU_I2C, ADDRESS_IMU_WHO_AM_I, &output, 1);
    status &= (output == VALUE_IMU_WHO_AM_I);

    
}