#include "uart0.h"

static volatile uint8_t rxBuffer[UART0_RX_BUFFER];
static volatile uint16_t rxHead = 0;
static volatile uint16_t rxTail = 0;
static volatile bool rxOverflow = false;

static void UART0_ReceiveCallback(void) {
    uint8_t data;
    uint16_t next;

    while (uart_is_readable(uart0)) {
        data = (uint8_t)uart_getc(uart0);
        next = (rxHead + 1) % UART0_RX_BUFFER;

        if (next != rxTail) {
            rxBuffer[rxHead] = data;
            rxHead = next;
        }
        else {
            rxOverflow = true;
        }
    }
}

bool UART0_Initialize(void) {
    uint32_t baud;

    gpio_set_function(PIN_UART0_TX, UART_FUNCSEL_NUM(uart0, PIN_UART0_TX));
    gpio_set_function(PIN_UART0_RX, UART_FUNCSEL_NUM(uart0, PIN_UART0_RX));

    gpio_pull_up(PIN_UART0_RX);

    baud = uart_init(uart0, UART0_BAUD);
    if ((baud < (UART0_BAUD * 0.98f)) || (baud > (UART0_BAUD * 1.02f))) {
        return false;
    }

    uart_set_format(uart0, UART0_DATA_BITS, UART0_STOP_BITS, UART_PARITY_NONE);
    uart_set_hw_flow(uart0, false, false);
    uart_set_fifo_enabled(uart0, true);

    irq_set_exclusive_handler(UART0_IRQ, UART0_ReceiveCallback);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irqs_enabled(uart0, true, false);

    return true;
}

bool UART0_ChangeBaud(const uint32_t baud) {
    uint32_t actual;

    if (!uart_is_enabled(uart0)) {
        return false;
    }

    uart_tx_wait_blocking(uart0);
    irq_set_enabled(UART0_IRQ, false);

    actual = uart_set_baudrate(uart0, baud);

    irq_set_enabled(UART0_IRQ, true);

    if ((actual < (baud * 0.98f)) || (actual > (baud * 1.02f))) {
        return false;
    }

    return true;
}

void UART0_Write(const uint8_t *data, const uint16_t length) {
    uart_write_blocking(uart0, data, length);
}

bool UART0_Read(uint8_t *data) {
    if (rxTail == rxHead) {
        return false;
    }

    *data = rxBuffer[rxTail];
    rxTail = (rxTail + 1) % UART0_RX_BUFFER;

    return true;
}

bool UART0_IsOverflowed(void) {
    return rxOverflow;
}

void UART0_Flush(void) {
    rxTail = rxHead;
    rxOverflow = false;
}
