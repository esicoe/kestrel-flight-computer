/**
 * @file    config.h
 * @brief   Libraries,
 *          Bus speeds,
 *          Helpers.
 * @author  esicoe
 */

#ifndef CONFIG_H
#define CONFIG_H

// Standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>

// PICO SDK
#include "pico/stdlib.h"            
#include "pico/multicore.h"         
#include "pico/util/queue.h"        
#include "pico/critical_section.h"  
#include "hardware/i2c.h"           
#include "hardware/spi.h"           
#include "hardware/uart.h"          
#include "hardware/dma.h"           
#include "hardware/pwm.h"           
#include "hardware/watchdog.h"      
#include "hardware/irq.h"

// CLOCKS           
#define I2C0_CLOCK  1000000UL   // 1 MHz
#define I2C1_CLOCK  400000UL    // 400 kHz
#define SPI0_CLOCK  15000000UL  // 15 MHz
#define SPI1_CLOCK  400000UL    // 400 kHz
#define UART0_BAUD  9600UL      // 9.6 KBd
#define UART1_BAUD  115200UL    // 115.2 KBd

// HELPERS
#define PYRO_OFF        false
#define PYRO_ON         true
#define LED_OFF         false
#define LED_ON          true
#define PIN_LOW         false
#define PIN_HIGH        true     

// 8-BIT REGISTER PROGRAMMING STRUCT
typedef struct {
    uint8_t reg;
    uint8_t value;
} register_setting_t;

#endif // CONFIG_H