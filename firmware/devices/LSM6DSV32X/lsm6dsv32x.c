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