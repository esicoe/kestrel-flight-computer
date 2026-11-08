#include "maxm10s.h"

bool MAXM10S_Configure(void) {
    gpio_init(PIN_GNSS_RST);
    gpio_disable_pulls(PIN_GNSS_RST);
    gpio_put(PIN_GNSS_RST, PIN_LOW);
    gpio_set_dir(PIN_GNSS_RST, GPIO_OUT);

    sleep_ms(VALUE_GNSS_RESET_MS);

    gpio_set_dir(PIN_GNSS_RST, GPIO_IN);

    sleep_ms(VALUE_GNSS_BOOT_MS);

    UART0_Flush();

    return true;
}

bool MAXM10S_ReadLine(char *line, const uint16_t size, const uint32_t timeout_ms) {
    uint64_t deadline = time_us_64() + ((uint64_t)timeout_ms * 1000);
    uint16_t length = 0;
    uint8_t data;
    bool overflow = false;

    if (size < 2) {
        return false;
    }

    if (UART0_IsOverflowed()) {
        UART0_Flush();
        overflow = true;
    }

    while (time_us_64() < deadline) {
        if (UART0_IsOverflowed()) {
            UART0_Flush();
            overflow = true;
            length = 0;
        }

        if (!UART0_Read(&data)) {
            continue;
        }

        if (data == '\n') {
            line[length] = '\0';

            if (overflow || (length == 0)) {
                length = 0;
                overflow = false;
                continue;
            }

            return true;
        }

        if ((data == '\r') || (length >= (size - 1))) {
            if (length >= (size - 1)) {
                overflow = true;
            }

            continue;
        }

        line[length] = (char)data;
        length++;
    }

    return false;
}
