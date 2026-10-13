/**
 * @file    pins.h
 * @brief   RP2354 pin definitions.
 * @author  esicoe
 */

#ifndef PINS_H
#define PINS_H

#define PIN_SPI0_MOSI   19U // PIN 19
#define PIN_SPI0_MISO   16U // PIN 16
#define PIN_SPI0_SCK    18U // PIN 18
#define PIN_SPI0_CS     17U // PIN 17

#define PIN_SPI1_MOSI   31U // PIN 39
#define PIN_SPI1_MISO   28U // PIN 36
#define PIN_SPI1_SCK    30U // PIN 38
#define PIN_SPI1_CS     29U // PIN 37

#define PIN_I2C0_SCL    9U  // PIN 7
#define PIN_I2C0_SDA    8U  // PIN 6
#define PIN_IMU_INT     7U  // PIN 4

#define PIN_I2C1_SCL    11U // PIN 9
#define PIN_I2C1_SDA    10U // PIN 8

#define PIN_UART0_TX    0U  // PIN 77
#define PIN_UART0_RX    1U  // PIN 78
#define PIN_GNSS_RST    2U  // PIN 79

#define PIN_UART1_TX    24U // PIN 25
#define PIN_UART1_RX    25U // PIN 26
#define PIN_LORA_RST    20U // PIN 20

#define PIN_SERVO1_PWM  32U // PIN 40
#define PIN_SERVO2_PWM  36U // PIN 45

#define PIN_PYRO1       43U // PIN 54
#define PIN_PYRO2       44U // PIN 55
#define PIN_PYRO3       45U // PIN 56

#define PIN_LED_STATUSA 12U // PIN 11
#define PIN_LED_STATUSB 13U // PIN 12
#define PIN_LED_STATUSC 14U // PIN 13

#endif // PINS_H