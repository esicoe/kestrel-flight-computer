/**
 * @file    uart0.h
 * @brief   UART0 driver.
 * @author  esicoe
 */

#ifndef UART0_H
#define UART0_H

#include "config.h"
#include "pins.h"

/**
 * @brief   Initializes UART0.
 *          Sets GPIO function,
 *          Pulls RX up (line stays idle while the MAX-M10S resets),
 *          Sets baud rate,
 *          Sets frame format (8N1, no flow control),
 *          Enables the RX interrupt that fills the RX ring buffer.
 * @return  TRUE/FALSE if the baud rate is within 2% of UART0_BAUD.
 */
bool UART0_Initialize(void);

/**
 * @brief   Changes the baud rate.
 * @param   baud        Desired baud rate.
 * @return  TRUE/FALSE if the baud rate is within 2% of the desired one.
 * @note    Waits for TX to drain and masks UART0_IRQ during the change.
 */
bool UART0_ChangeBaud(const uint32_t baud);

/**
 * @brief   Writes bytes.
 * @param   data        Write content.
 * @param   length      Number of bytes to write.
 * @note    Blocks while the 32B TX FIFO is full, about 1042us per byte beyond the first 32 at 9.6 KBd.
 */
void UART0_Write(const uint8_t *data, const uint16_t length);

/**
 * @brief   Reads one byte from the RX ring buffer.
 * @param   data        Address of outputted byte.
 * @return  TRUE if a byte was read,
 *          FALSE if the buffer is empty.
 * @note    Single consumer: call UART0_Read() and UART0_Flush() from the core that ran UART0_Initialize().
 */
bool UART0_Read(uint8_t *data);

/**
 * @brief   Checks if bytes were dropped because the RX ring buffer (UART0_RX_BUFFER) was full.
 * @return  TRUE if bytes were dropped since the last UART0_Flush(),
 *          FALSE otherwise.
 */
bool UART0_IsOverflowed(void);

/**
 * @brief   Discards every byte in the RX ring buffer and clears the overflow flag.
 */
void UART0_Flush(void);

#endif // UART0_H
