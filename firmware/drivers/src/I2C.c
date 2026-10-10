#include "I2C.h"

bool I2C_INIT(i2c_inst_t *i2c) {
    unsigned long CLOCK;

    if (i2c == i2c0) {
        CLOCK = i2c_init(i2c0, I2C0_CLOCK);
        if (CLOCK != I2C0_CLOCK) return false;

        gpio_set_function(PIN_I2C0_SCL, GPIO_FUNC_I2C);    
        gpio_set_function(PIN_I2C0_SDA, GPIO_FUNC_I2C);
        
        gpio_disable_pulls(PIN_I2C0_SCL);
        gpio_disable_pulls(PIN_I2C0_SDA);
        
        return true;
    }
    else {
        CLOCK = i2c_init(i2c1, I2C1_CLOCK);
        if (CLOCK != I2C1_CLOCK) return false;

        gpio_set_function(PIN_I2C1_SCL, GPIO_FUNC_I2C);    
        gpio_set_function(PIN_I2C1_SDA, GPIO_FUNC_I2C);
        
        gpio_disable_pulls(PIN_I2C1_SCL);
        gpio_disable_pulls(PIN_I2C1_SDA);

        return true;
    }
}

bool I2C_WRITE(i2c_inst_t *i2c, uint8_t address, uint8_t reg, uint8_t data) {
    int STATUS;
    uint8_t buffer[2] = {reg, data};

    STATUS = i2c_write_timeout_us(i2c, address, buffer, 2, false, 1000);
    if (STATUS != 2) return false;
    return true;
}

bool I2C_READ(i2c_inst_t *i2c, uint8_t address, uint8_t reg, uint8_t *data, int length) {
    int STATUS;

    STATUS = i2c_write_timeout_us(i2c, address, &reg, 1, true, 1000);
    if (STATUS != 1) return false;

    STATUS = i2c_read_timeout_us(i2c, address, data, length, false, 1000 * length);
    if (STATUS != length) return false;
    return true;
}