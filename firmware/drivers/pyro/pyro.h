/**
 * @file    pyro.h
 * @brief   Pyro channel driver.
 * @author  esicoe
 */

#ifndef PYRO_H
#define PYRO_H

#include "config.h"
#include "pins.h"

typedef enum {
    PYRO_1  = 0,
    PYRO_2  = 1,
    PYRO_3  = 2
} pyro_channel_t;

/**
 * @brief   Initializes the pyro gate GPIOs as outputs, all channels off.
 * @note    Replaces the pyro setup at the top of main(): call it first.
 */
void PYRO_Initialize(void);

/**
 * @brief   Turns a channel on, a timer alarm turns it off after the duration.
 * @param   channel     pyro_channel_t.
 * @param   duration    On-time (ms), 1 to PYRO_MAX_MS.
 * @return  TRUE if the channel fired,
 *          FALSE on invalid channel or duration, channel already on, or no free alarm (channel left off).
 * @note    Non-blocking. One call gives one pulse of at most PYRO_MAX_MS: a channel already on is not restarted.
 *          The off alarm runs in the timer IRQ of core 0: don't block interrupts for long,
 *          and enable the watchdog in main() as a backup. Call from core 0 thread code, not from an IRQ.
 *          No continuity sensing: the board's red LEDs only show continuity locally.
 */
bool PYRO_Fire(const pyro_channel_t channel, const uint32_t duration);

/**
 * @brief   Turns a channel off immediately and cancels its timer alarm.
 * @param   channel     pyro_channel_t.
 */
void PYRO_Off(const pyro_channel_t channel);

#endif // PYRO_H