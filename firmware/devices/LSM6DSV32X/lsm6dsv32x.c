#include "lsm6dsv32x.h"

static register_setting_t lsm6dsv32xConfig[] = {
    {ADDRESS_IMU_CTRL3,     VALUE_IMU_BDU         | VALUE_IMU_IF_INC                                },
    {ADDRESS_IMU_CTRL4,     VALUE_IMU_DRDY_MASK                                                     }, 
    {ADDRESS_IMU_CTRL8,     VALUE_IMU_FS_XL_16    | VALUE_IMU_CTRL8_RESERVED | VALUE_IMU_LPF2_XL_20 }, 
    {ADDRESS_IMU_CTRL9,     VALUE_IMU_LPF2_XL_EN                                                    },  
    {ADDRESS_IMU_CTRL6,     VALUE_IMU_FS_G_1000   | VALUE_IMU_LPF1_G_4                              }, 
    {ADDRESS_IMU_CTRL7,     VALUE_IMU_LPF1_G_EN                                                     }, 
    {ADDRESS_IMU_INT1_CTRL, VALUE_IMU_INT1_DRDY_G                                                   }, 
    {ADDRESS_IMU_CTRL1,     VALUE_IMU_ODR_XL_1920 | VALUE_IMU_MODE_XL_HP                            }, 
    {ADDRESS_IMU_CTRL2,     VALUE_IMU_ODR_G_1920  | VALUE_IMU_MODE_G_HP                             },
};

void LSM6DSV32X_Initialize(void) {
    bool status = true;
    uint8_t output;
    uint8_t input;

    status &= I2C0_Read(ADDRESS_IMU_I2C, ADDRESS_IMU_WHO_AM_I, &output, 1);
    status &= (output == VALUE_IMU_WHO_AM_I);

    input = VALUE_IMU_SW_RESET;
    status &= I2C0_Write(ADDRESS_IMU_I2C, ADDRESS_IMU_CTRL3, input);

    sleep_ms(50);

    status &= I2C0_Read(ADDRESS_IMU_I2C, ADDRESS_IMU_CTRL3, &output, 1);
    status &= !(output & VALUE_IMU_SW_RESET);

    for (uint8_t i = 0; i < (sizeof(lsm6dsv32xConfig) / sizeof(lsm6dsv32xConfig[0])); i++) {
        input = lsm6dsv32xConfig[i].value;
        status &= I2C0_Write(ADDRESS_IMU_I2C, lsm6dsv32xConfig[i].reg, input);
    }

    for (uint8_t i = 0; i < (sizeof(lsm6dsv32xConfig) / sizeof(lsm6dsv32xConfig[0])); i++) {
        status &= I2C0_Read(ADDRESS_IMU_I2C, lsm6dsv32xConfig[i].reg, &output, 1);
        status &= (output == lsm6dsv32xConfig[i].value);
    }

    if (!status) {
        LED_DisplayError(IMU_FAIL);
    }
}

lsm6dsv32x_output_t LSM6DSV32X_Read(void) {
    lsm6dsv32x_output_t output = {0};
    uint8_t buffer[12];
    uint8_t crc = 0xFF;

    output.time_us = time_us_64();

    bool status = I2C0_Read(ADDRESS_IMU_I2C, ADDRESS_IMU_OUTX_LSB_G, buffer, 12);
    if (!status) {
        output.error = true;

        return output;
    }

    float gyro_sx  = (int16_t)(((uint16_t)buffer[1]  << 8) | buffer[0])  * VALUE_IMU_GYRO_SCALE_1000;
    float gyro_sy  = (int16_t)(((uint16_t)buffer[3]  << 8) | buffer[2])  * VALUE_IMU_GYRO_SCALE_1000;
    float gyro_sz  = (int16_t)(((uint16_t)buffer[5]  << 8) | buffer[4])  * VALUE_IMU_GYRO_SCALE_1000;
    float accel_sx = (int16_t)(((uint16_t)buffer[7]  << 8) | buffer[6])  * VALUE_IMU_ACCEL_SCALE_16;
    float accel_sy = (int16_t)(((uint16_t)buffer[9]  << 8) | buffer[8])  * VALUE_IMU_ACCEL_SCALE_16;
    float accel_sz = (int16_t)(((uint16_t)buffer[11] << 8) | buffer[10]) * VALUE_IMU_ACCEL_SCALE_16;

    output.gyro_x_dps  = -gyro_sy;
    output.gyro_y_dps  = -gyro_sx;
    output.gyro_z_dps  = -gyro_sz;
    output.accel_x_ms2 = -accel_sy;
    output.accel_y_ms2 = -accel_sx;
    output.accel_z_ms2 = -accel_sz;

    output.error = false;

    uint8_t *output_bytes = (uint8_t *)&output;

    for (uint8_t i = 0; i < offsetof(lsm6dsv32x_output_t, crc8); i++) {
        crc = crc ^ output_bytes[i];

        for (uint8_t j = 0; j < 8; j++) {
            if (crc >> 7) {
                crc = (crc << 1) ^ 0x31;
            }
            else {
                crc = crc << 1;
            }
        }
    }

    output.crc8 = crc;

    return output;
}