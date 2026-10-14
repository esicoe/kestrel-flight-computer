#include "bmp585.h"

static register_setting_t bmp585Config[] = {
    {ADDRESS_BARO_DSP_CONFIG, VALUE_BARO_DSP_RESERVED | VALUE_BARO_DSP_SHDW_IIR_P                  },
    {ADDRESS_BARO_DSP_IIR,    VALUE_BARO_IIR_P_3      | VALUE_BARO_IIR_T_0                         },
    {ADDRESS_BARO_OSR_CONFIG, VALUE_BARO_PRESS_ON     | VALUE_BARO_OSR_P_8 | VALUE_BARO_OSR_T_1    },
    {ADDRESS_BARO_ODR_CONFIG, VALUE_BARO_DEEP_DISABLE | VALUE_BARO_ODR_140 | VALUE_BARO_MODE_NORMAL},
};

void BMP585_Initialize(void) {
    bool status = true;
    uint8_t output;
    uint8_t input;

    status &= I2C1_Read(ADDRESS_BARO_I2C, ADDRESS_BARO_CHIP_ID, &output, 1);
    status &= (output == VALUE_BARO_CHIP_ID);

    status &= I2C1_Read(ADDRESS_BARO_I2C, ADDRESS_BARO_STATUS, &output, 1);
    status &= ((output & 0x06)  == VALUE_BARO_STATUS_RDY);

    input = VALUE_BARO_DEEP_DISABLE | (bmp585Config[3].value & 0x7C) | VALUE_BARO_MODE_STANDBY;
    status &= I2C1_Write(ADDRESS_BARO_I2C, ADDRESS_BARO_ODR_CONFIG, input);

    sleep_ms(5);

    for (uint8_t i = 0; i < (sizeof(bmp585Config) / sizeof(bmp585Config[0])); i++) {
        input = bmp585Config[i].value;
        status &= I2C1_Write(ADDRESS_BARO_I2C, bmp585Config[i].reg, input);
    }

    sleep_ms(5);

    status &= I2C1_Read(ADDRESS_BARO_I2C, ADDRESS_BARO_OSR_EFF, &output, 1);
    status &= (output == (VALUE_BARO_ODR_IS_VALID | (bmp585Config[2].value & 0x3F)));

    if (!status) {
        LED_DisplayError(BAROMETER_FAIL); 
    }
}

bmp585_output_t BMP585_Read(void) {
    bmp585_output_t output = {0};
    uint8_t buffer[6];
    uint8_t crc = 0xFF;

    output.time_us = time_us_64();

    bool status = I2C1_Read(ADDRESS_BARO_I2C, ADDRESS_BARO_TEMP_DATA_XLSB, buffer, 6);
    if (!status) {
        output.error = true;

        return output;
    }

    output.pressure_Pa = (((uint32_t)buffer[5] << 16) | ((uint32_t)buffer[4] << 8) | buffer[3]) / 64.0f;
    output.temperature_K = (uint16_t)(((((int32_t)(((uint32_t)buffer[2] << 24) | ((uint32_t)buffer[1] << 16) | ((uint32_t)buffer[0] << 8))) >> 8) + 17901158 + 32768) / 65536);    
    output.error = false;

    uint8_t *output_bytes = (uint8_t *)&output;

    for(uint8_t i = 0; i < 15; i++) {
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