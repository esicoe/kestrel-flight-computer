/**
 * @file    led.h
 * @brief   Status LED driver.
 * @author  esicoe
 */

#ifndef LED_H
#define LED_H

#include "config.h"
#include "pins.h"

typedef enum {
    BAROMETER_FAIL  = 0b00000001,
    IMU_FAIL        = 0b00000010,
    FLASH_FAIL      = 0b00000100,
    MicroSD_FAIL    = 0b00000011,
    LORA_FAIL       = 0b00000110,
    GNSS_FAIL       = 0b00000111,
    PASS_1          = 0b00001000,
    PASS_2          = 0b00010000,
    PASS_3          = 0b00100000,
    PASS_4          = 0b00011000,
    PASS_5          = 0b00110000,
    PASS_6          = 0b00111000
} led_code_t;

/**
 * @brief   Initializes LEDs GPIO and 250ms timer.
 */
void LED_Initialize(void);

/**
 * @brief   Error code, solid LEDs and blocks CPU.
 * @param   status      led_code_t.
 * @note    Never returns.
 */
void LED_DisplayError(uint8_t status);

/**
 * @brief   Status code, blinking LEDs.
 * @param   status      led_code_t.
 */
void LED_DisplayStatus(uint8_t status);

#endif // LED_H
