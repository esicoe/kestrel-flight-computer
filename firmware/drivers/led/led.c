#include "led.h"

static repeating_timer_t timer250ms;
static volatile uint8_t currentStatus = 0;

static bool blinkCallback(repeating_timer_t *rt) {
    (void)rt;
    static bool onOff;

    uint8_t status = currentStatus;

    if (status & 0b00001000) gpio_put(PIN_LED_STATUSA, onOff ? LED_ON : LED_OFF);
    if (status & 0b00010000) gpio_put(PIN_LED_STATUSB, onOff ? LED_ON : LED_OFF);
    if (status & 0b00100000) gpio_put(PIN_LED_STATUSC, onOff ? LED_ON : LED_OFF);

    onOff = !onOff;

    return true;
}

void LED_Initialize(void) {
    gpio_init(PIN_LED_STATUSA);
    gpio_init(PIN_LED_STATUSB);
    gpio_init(PIN_LED_STATUSC);

    gpio_set_dir(PIN_LED_STATUSA, GPIO_OUT);
    gpio_set_dir(PIN_LED_STATUSB, GPIO_OUT);
    gpio_set_dir(PIN_LED_STATUSC, GPIO_OUT);

    gpio_put(PIN_LED_STATUSA, LED_OFF);
    gpio_put(PIN_LED_STATUSB, LED_OFF);
    gpio_put(PIN_LED_STATUSC, LED_OFF);

    add_repeating_timer_ms(250, blinkCallback, NULL, &timer250ms);
}

void LED_DisplayError(uint8_t status) {
    currentStatus = status;

    cancel_repeating_timer(&timer250ms);

    gpio_put(PIN_LED_STATUSA, (status & 0b00000001) ? LED_ON : LED_OFF);
    gpio_put(PIN_LED_STATUSB, (status & 0b00000010) ? LED_ON : LED_OFF);
    gpio_put(PIN_LED_STATUSC, (status & 0b00000100) ? LED_ON : LED_OFF);

    while (true) {}     
}

void LED_DisplayStatus(uint8_t status) {
    currentStatus = status;

    if (!(status & 0b00001000)) gpio_put(PIN_LED_STATUSA, (status & 0b00000001) ? LED_ON : LED_OFF);
    if (!(status & 0b00010000)) gpio_put(PIN_LED_STATUSB, (status & 0b00000010) ? LED_ON : LED_OFF);
    if (!(status & 0b00100000)) gpio_put(PIN_LED_STATUSC, (status & 0b00000100) ? LED_ON : LED_OFF);
}