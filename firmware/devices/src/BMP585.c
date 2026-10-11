#include "BMP585.h"

void BMP585_INIT(void) {
    bool STATUS = true;
    uint8_t output = 0;
    uint8_t input = 0;

    STATUS &= I2C_READ(i2c1, ADDRESS_BARO_I2C, ADDRESS_BARO_CHIP_ID, &output, 1);
    STATUS &= (output == VALUE_BARO_CHIP_ID);

    STATUS &= I2C_READ(i2c1, ADDRESS_BARO_I2C, ADDRESS_BARO_STATUS, &output, 1);
    STATUS &= ((output & 0x06)  == VALUE_BARO_STATUS_RDY);

    input = VALUE_BARO_DEEP_DISABLE | (BMP585[3].value & 0x7C) | VALUE_BARO_MODE_STANDBY;
    STATUS &= I2C_WRITE(i2c1, ADDRESS_BARO_I2C, ADDRESS_BARO_ODR_CONFIG, input);

    sleep_ms(3);

    for (uint8_t i = 0; i < (sizeof(BMP585) / sizeof(BMP585[0])); i++) {
        input = BMP585[i].value;
        STATUS &= I2C_WRITE(i2c1, ADDRESS_BARO_I2C, BMP585[i].reg, input);
    }

    sleep_ms(3);

    STATUS &= I2C_READ(i2c1, ADDRESS_BARO_I2C, ADDRESS_BARO_OSR_EFF, &output, 1);
    STATUS &= (output == (VALUE_BARO_ODR_IS_VALID | (BMP585[2].value & 0x3F)));

    if (!STATUS) {
        // console display function here
        DISPLAY_ERROR(BAROMETER_FAIL); 
    }
}

struct BARO_OUTPUT BARO_READ(void) {
    struct BARO_OUTPUT output = {0};
    uint8_t buffer[6];

    output.time_us = time_us_64();

    bool STATUS = I2C_READ(i2c1, ADDRESS_BARO_I2C, ADDRESS_BARO_TEMP_DATA_XLSB, buffer, 6);
    if (!STATUS) {
        output.error = true;

        return output;
    }

    output.pressure_Pa = (((uint32_t)buffer[5] << 16) | ((uint32_t)buffer[4] << 8) | buffer[3]) / 64.0f;
    output.temperature_cK = (uint16_t)((((int32_t)(((uint32_t)buffer[2] << 24) | ((uint32_t)buffer[1] << 16) | ((uint32_t)buffer[0] << 8)) >> 8) * 100) / 65536);
    output.error = false;

    // Checksum
    output.checksum = 

    return output;
}