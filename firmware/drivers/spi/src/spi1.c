#include "spi1.h"

bool SPI1_Initialize(void) {
    uint32_t clock;

    gpio_init(PIN_SPI1_CS);
    gpio_disable_pulls(PIN_SPI1_CS);
    gpio_put(PIN_SPI1_CS, PIN_HIGH);
    gpio_set_dir(PIN_SPI1_CS, GPIO_OUT);

    gpio_set_function(PIN_SPI1_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SPI1_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SPI1_SCK, GPIO_FUNC_SPI);

    gpio_pull_up(PIN_SPI1_MISO);

    clock = spi_init(spi1, SPI1_CLOCK);
    if ((clock < (SPI1_CLOCK * 0.98f)) || (clock > (SPI1_CLOCK * 1.02f))) {
        return false;
    }

    spi_set_format(spi1, SPI1_DATA_BITS, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    return true;
}

bool SPI1_ChangeClock(const uint32_t clock) {
    uint32_t actual = spi_set_baudrate(spi1, clock);

    if ((actual < (clock * 0.98f)) || (actual > (clock * 1.02f))) {
        return false;
    }

    return true;
}

void SPI1_Select(void) {
    gpio_put(PIN_SPI1_CS, PIN_LOW);
}

void SPI1_Deselect(void) {
    gpio_put(PIN_SPI1_CS, PIN_HIGH);
}

uint8_t SPI1_Transfer(const uint8_t data) {
    uint8_t output;

    spi_write_read_blocking(spi1, &data, &output, 1);

    return output;
}

void SPI1_Write(const uint8_t *data, const uint16_t length) {
    spi_write_blocking(spi1, data, length);
}

void SPI1_Read(uint8_t *data, const uint16_t length) {
    spi_read_blocking(spi1, SPI_DUMMY_BYTE, data, length);
}