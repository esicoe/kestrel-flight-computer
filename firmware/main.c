#include "config.h"
#include "pins.h"

#include "pyro.h"
#include "console.h"

int main(void) {
    PYRO_Initialize();

    stdio_init_all();

    CONSOLE_Initialize();

    while(true) {
        CONSOLE_Update();
    }
}