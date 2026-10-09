#include "LED.h"

static bool BLINK(repeating_timer_t *rt) {
    static bool ON_OFF;

    uint8_t status = CURRENT_STATUS;

    if (status & 0b00001000) gpio_put(PIN_LED_STATUSA, ON_OFF ? LED_ON : LED_OFF);
    if (status & 0b00010000) gpio_put(PIN_LED_STATUSB, ON_OFF ? LED_ON : LED_OFF);
    if (status & 0b00100000) gpio_put(PIN_LED_STATUSC, ON_OFF ? LED_ON : LED_OFF);

    ON_OFF = !ON_OFF;

    return true;
}

void LED_INIT(void) {
    gpio_init(PIN_LED_STATUSA);
    gpio_init(PIN_LED_STATUSB);
    gpio_init(PIN_LED_STATUSC);

    gpio_set_dir(PIN_LED_STATUSA, GPIO_OUT);
    gpio_set_dir(PIN_LED_STATUSB, GPIO_OUT);
    gpio_set_dir(PIN_LED_STATUSC, GPIO_OUT);

    gpio_put(PIN_LED_STATUSA, LED_OFF);
    gpio_put(PIN_LED_STATUSB, LED_OFF);
    gpio_put(PIN_LED_STATUSC, LED_OFF);

    add_repeating_timer_ms(250, BLINK, NULL, &TIMER_250MS);
}

void DISPLAY_ERROR(uint8_t status) {
    CURRENT_STATUS = status;

    cancel_repeating_timer(&TIMER_250MS);

    gpio_put(PIN_LED_STATUSA, (status & 0b00000001) ? LED_ON : LED_OFF);
    gpio_put(PIN_LED_STATUSB, (status & 0b00000010) ? LED_ON : LED_OFF);
    gpio_put(PIN_LED_STATUSC, (status & 0b00000100) ? LED_ON : LED_OFF);

    while (true) {}     
}

void DISPLAY_STATUS(uint8_t status) {
    CURRENT_STATUS = status;

    if (!(status & 0b00001000)) gpio_put(PIN_LED_STATUSA, (status & 0b00000001) ? LED_ON : LED_OFF);
    if (!(status & 0b00010000)) gpio_put(PIN_LED_STATUSB, (status & 0b00000010) ? LED_ON : LED_OFF);
    if (!(status & 0b00100000)) gpio_put(PIN_LED_STATUSC, (status & 0b00000100) ? LED_ON : LED_OFF);
}