#include "config.h"
#include "pins.h"

int main(void) {
    gpio_init(PIN_PYRO1);
    gpio_init(PIN_PYRO2);
    gpio_init(PIN_PYRO3);

    gpio_put(PIN_PYRO1, PYRO_OFF);
    gpio_put(PIN_PYRO2, PYRO_OFF);
    gpio_put(PIN_PYRO3, PYRO_OFF);

    gpio_set_dir(PIN_PYRO1, GPIO_OUT);
    gpio_set_dir(PIN_PYRO2, GPIO_OUT);
    gpio_set_dir(PIN_PYRO3, GPIO_OUT);

    stdio_init_all();
    
    while(true) {}
}