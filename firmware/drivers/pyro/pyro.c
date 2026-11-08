#include "pyro.h"

static const uint8_t pyroPins[] = {PIN_PYRO1, PIN_PYRO2, PIN_PYRO3};
static volatile alarm_id_t pyroAlarms[] = {0, 0, 0};

static int64_t PYRO_OffCallback(alarm_id_t id, void *user_data) {
    (void)id;
    pyro_channel_t channel = (pyro_channel_t)(uintptr_t)user_data;

    gpio_put(pyroPins[channel], PYRO_OFF);
    pyroAlarms[channel] = 0;

    return 0;
}

void PYRO_Initialize(void) {
    for (uint8_t i = 0; i < (sizeof(pyroPins) / sizeof(pyroPins[0])); i++) {
        gpio_init(pyroPins[i]);
        gpio_put(pyroPins[i], PYRO_OFF);
        gpio_set_dir(pyroPins[i], GPIO_OUT);

        PYRO_Off((pyro_channel_t)i);
    }
}

bool PYRO_Fire(const pyro_channel_t channel, const uint32_t duration) {
    if ((channel > PYRO_3) || (duration == 0) || (duration > PYRO_MAX_MS)) {
        return false;
    }

    if (gpio_get_out_level(pyroPins[channel])) {
        return false;
    }

    gpio_put(pyroPins[channel], PYRO_ON);

    pyroAlarms[channel] = add_alarm_in_ms(duration, PYRO_OffCallback, (void *)(uintptr_t)channel, true);
    if (pyroAlarms[channel] <= 0) {
        gpio_put(pyroPins[channel], PYRO_OFF);
        pyroAlarms[channel] = 0;

        return false;
    }

    return true;
}

void PYRO_Off(const pyro_channel_t channel) {
    alarm_id_t alarm;

    if (channel > PYRO_3) {
        return;
    }

    alarm = pyroAlarms[channel];
    pyroAlarms[channel] = 0;

    if (alarm > 0) {
        cancel_alarm(alarm);
    }

    gpio_put(pyroPins[channel], PYRO_OFF);
}