#include "uart1.h"

static volatile uint8_t rxBuffer[UART1_RX_BUFFER];
static volatile uint16_t rxHead = 0;
static volatile uint16_t rxTail = 0;
static volatile bool rxOverflow = false;

static void UART1_ReceiveCallback(void) {
    uint8_t data;
    uint16_t next;

    while (uart_is_readable(uart1)) {
        data = (uint8_t)uart_getc(uart1);
        next = (rxHead + 1) % UART1_RX_BUFFER;

        if (next != rxTail) {
            rxBuffer[rxHead] = data;
            rxHead = next;
        }
        else {
            rxOverflow = true;
        }
    }
}

bool UART1_Initialize(void) {
    uint32_t baud;

    gpio_set_function(PIN_UART1_TX, UART_FUNCSEL_NUM(uart1, PIN_UART1_TX));
    gpio_set_function(PIN_UART1_RX, UART_FUNCSEL_NUM(uart1, PIN_UART1_RX));

    gpio_pull_up(PIN_UART1_RX);

    baud = uart_init(uart1, UART1_BAUD);
    if ((baud < (UART1_BAUD * 0.98f)) || (baud > (UART1_BAUD * 1.02f))) {
        return false;
    }

    uart_set_format(uart1, UART1_DATA_BITS, UART1_STOP_BITS, UART_PARITY_NONE);
    uart_set_hw_flow(uart1, false, false);
    uart_set_fifo_enabled(uart1, true);

    irq_set_exclusive_handler(UART1_IRQ, UART1_ReceiveCallback);
    irq_set_enabled(UART1_IRQ, true);
    uart_set_irqs_enabled(uart1, true, false);

    return true;
}

bool UART1_ChangeBaud(const uint32_t baud) {
    uint32_t actual;

    if (!uart_is_enabled(uart1)) {
        return false;
    }

    uart_tx_wait_blocking(uart1);
    irq_set_enabled(UART1_IRQ, false);

    actual = uart_set_baudrate(uart1, baud);

    irq_set_enabled(UART1_IRQ, true);

    if ((actual < (baud * 0.98f)) || (actual > (baud * 1.02f))) {
        return false;
    }

    return true;
}

void UART1_Write(const uint8_t *data, const uint16_t length) {
    uart_write_blocking(uart1, data, length);
}

bool UART1_Read(uint8_t *data) {
    if (rxTail == rxHead) {
        return false;
    }

    *data = rxBuffer[rxTail];
    rxTail = (rxTail + 1) % UART1_RX_BUFFER;

    return true;
}

bool UART1_IsOverflowed(void) {
    return rxOverflow;
}

void UART1_Flush(void) {
    rxTail = rxHead;
    rxOverflow = false;
}