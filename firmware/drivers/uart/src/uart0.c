#include "uart0.h"

bool UART0_Initialize(void) {
    unsigned long baud;

    gpio_init(PIN_UART0_RX);
    gpio_init(PIN_UART0_TX);

    gpio_set_function(PIN_UART0_TX, UART_FUNCSEL_NUM(uart0, PIN_UART0_TX));
    gpio_set_function(PIN_UART0_RX, UART_FUNCSEL_NUM(uart0, PIN_UART0_RX));

    uart_init
}