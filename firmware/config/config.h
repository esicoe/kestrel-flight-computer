/**
 * @file    config.h
 * @brief   Libaries,
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
#define I2C0_CLOCK  400000UL    // 400 KHz
#define I2C1_CLOCK  400000UL    // 400 KHz
#define SPI0_CLOCK  20000000UL  // 20 MHz
#define SPI1_CLOCK  400000UL    // 400 KHz
#define UART0_BAUD  9600UL      // 9.6 KBd
#define UART1_BAUD  115200UL    // 115.2 Bd

// HELPERS
#define PYRO_OFF                                    false
#define PYRO_ON                                     true
#define LED_OFF                                     false
#define LED_ON                                      true

#endif // CONFIG_H