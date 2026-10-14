#include "i2c0.h"

bool I2C0_Initialize(void) {
    unsigned int clock = i2c_init(i2c0, I2C0_CLOCK);
    if (clock != I2C0_CLOCK) {
        return false;
    }

    gpio_set_function(PIN_I2C0_SCL, GPIO_FUNC_I2C);
    gpio_set_function(PIN_I2C0_SDA, GPIO_FUNC_I2C);

    gpio_disable_pulls(PIN_I2C0_SCL);
    gpio_disable_pulls(PIN_I2C0_SDA);

    return true;
}

bool I2C0_Write(uint8_t address, uint8_t reg, uint8_t data) {
    int status;
    uint8_t buffer[2] = {reg, data};

    status = i2c_write_timeout_us(i2c0, address, buffer, 2, false, 1000);
    if (status != 2) {
        return false;
    }
    
    return true;
}

bool I2C0_Read(uint8_t address, uint8_t reg, uint8_t *data, int length) {
    int status;

    status = i2c_write_timeout_us(i2c0, address, &reg, 1, true, 1000);
    if (status != 1) return false;

    status = i2c_read_timeout_us(i2c0, address, data, length, false, 1000 * length);
    if (status != length) return false;
    return true;
}
