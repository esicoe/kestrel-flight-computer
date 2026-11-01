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

    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    return true;
}

