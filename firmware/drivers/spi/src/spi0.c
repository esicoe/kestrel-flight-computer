#include "spi0.h"

bool SPI0_Initialize(void) {
    uint32_t clock;

    gpio_init(PIN_SPI0_CS);
    gpio_disable_pulls(PIN_SPI0_CS);
    gpio_put(PIN_SPI0_CS, PIN_HIGH);
    gpio_set_dir(PIN_SPI0_CS, GPIO_OUT);

    gpio_set_function(PIN_SPI0_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SPI0_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SPI0_SCK, GPIO_FUNC_SPI);

    clock = spi_init(spi0, SPI0_CLOCK);
    if (clock != SPI0_CLOCK) {
        return false;
    }

    spi_set_format(spi0, SPI0_DATA_BITS, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    return true;
}

static void SPI0_Deselect(void) {
    gpio_put(PIN_SPI0_CS, PIN_HIGH);
}

static void SPI0_Select(void) {
    gpio_put(PIN_SPI0_CS, PIN_LOW);
}

void SPI0_Write(const uint8_t *data, const uint16_t length) {
    SPI0_Select();

    spi_write_blocking(spi0, data, length);

    SPI0_Deselect();
}

void SPI0_WriteRead(const uint8_t *writeData, const uint16_t writeLength, uint8_t *readData, const uint16_t readLength) {
    SPI0_Select();

    spi_write_blocking(spi0, writeData, writeLength);

    spi_read_blocking(spi0, SPI_DUMMY_BYTE, readData, readLength);

    SPI0_Deselect();
}