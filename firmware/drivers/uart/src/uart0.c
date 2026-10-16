#include "uart0.h"

bool UART0_Initialize(void) {
    gpio_init(PIN_UART0_RX);
    gpio_init(PIN_UART0_TX);

    gpio_set_function(PIN_UART0_TX, UART_FUNCSEL_NUM(uart0, PIN_UART0_TX));
    gpio_set_function(PIN_UART0_RX, UART_FUNCSEL_NUM(uart0, PIN_UART0_RX));

    unsigned int baud = uart_init(uart0, UART0_BAUD);
    if (!(((UART0_BAUD*0.98f) <= baud) && (baud <= (UART0_BAUD*1.02f)))) {
        return false;
    }

    return true;
}

bool UART0_ChangeBaud(unsigned int baud) {
    bool status = uart_is_enabled(uart0);
    if (!status) {
        return false;
    }

    unsigned int actual_baud = uart_set_baudrate(uart0, baud);

    if (((baud*0.98f) <= actual_baud) && (actual_baud <= baud*1.02f)) {
        return true;
    }

    return false;
}

void UART0_Write(const uint8_t *data, unsigned int length) {
    uart_write_blocking(uart0, data, length);
}

void UART0_Read(uint8_t* data) {
        
}
