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
#include <string.h>
#include <errno.h>

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
#define UART0_BAUD  9600UL      // 9.6 KBd (MAX-M10S default)
#define UART1_BAUD  115200UL    // 115.2 KBd

// FORMATS
#define SPI0_DATA_BITS  8U      // 8-bit frames
#define SPI1_DATA_BITS  8U      // 8-bit frames
#define UART0_DATA_BITS 8U      // 8-bit frames
#define UART0_STOP_BITS 1U      // 1 stop bit
#define UART1_DATA_BITS 8U      // 8-bit frames
#define UART1_STOP_BITS 1U      // 1 stop bit

// BUFFERS
#define UART0_RX_BUFFER 1024U   // 1 KB RX ring buffer
#define UART1_RX_BUFFER 1024U   // 1 KB RX ring buffer
#define CONSOLE_BUFFER  1100U   // 1.1 KB command line (sd write of 512 B)
#define CONSOLE_WORDS   4U      // 4 words per command

// LIMITS
#define PYRO_MAX_MS     2000U   // 2 s longest pyro pulse

// HELPERS
#define PYRO_OFF        false
#define PYRO_ON         true
#define LED_OFF         false
#define LED_ON          true
#define PIN_LOW         false
#define PIN_HIGH        true     
#define SPI_DUMMY_BYTE  0xFF

// 8-BIT REGISTER PROGRAMMING STRUCT
typedef struct {
    uint8_t reg;
    uint8_t value;
} register_setting_t;

#endif // CONFIG_H