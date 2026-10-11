#include "UART.h"

bool UART_INIT(uart_inst_t *uart) {
    unsigned long CLOCK;

    if (uart == uart0) {
        CLOCK = uart_init(uart0, UART0_BAUD);
        if (CLOCK != UART0_BAUD) return false;

        gpio_set_function(PIN_UART0_RX, UART_FUNCSEL_NUM(uart0,1));
        gpio_set_function(PIN_UART0_TX, UART_FUNCSEL_NUM(uart0,0));
        uart_set_format(uart0, 8, 1, 0);
    }
    else {
        CLOCK = uart_init(uart1, UART1_BAUD);
        if (CLOCK != UART1_BAUD) return false;

        gpio_set_function(PIN_UART1_RX, UART_FUNCSEL_NUM(uart1,1));
        gpio_set_function(PIN_UART1_TX, UART_FUNCSEL_NUM(uart1,0));
        uart_set_format(uart1, 8, 1, 0);
    }

    return true;
}

bool UART_CHANGE_BAUD(uart_inst_t *uart, unsigned long baud) {
    unsigned long CLOCK;

    CLOCK = uart_set_baudrate(uart, (unsigned int)baud);
    if (CLOCK != baud) return false;

    return true;
}

bool UART_TX(uart_inst_t *uart, char *data) {
    bool STATUS = true;

    STATUS = uart_is_writable(uart);
    if (!STATUS) return false;

    uart_puts(uart, data);
    return true;
}