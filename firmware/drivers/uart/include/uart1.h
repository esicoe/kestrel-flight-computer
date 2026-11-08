/**
 * @file    uart1.h
 * @brief   UART1 driver.
 * @author  esicoe
 */

#ifndef UART1_H
#define UART1_H

#include "config.h"
#include "pins.h"

/**
 * @brief   Initializes UART1.
 *          Sets GPIO function,
 *          Pulls RX up (line stays idle while the RAK3172 resets),
 *          Sets baud rate,
 *          Sets frame format (8N1, no flow control),
 *          Enables the RX interrupt that fills the RX ring buffer.
 * @return  TRUE/FALSE if the baud rate is within 2% of UART1_BAUD.
 */
bool UART1_Initialize(void);

/**
 * @brief   Changes the baud rate.
 * @param   baud        Desired baud rate.
 * @return  TRUE/FALSE if the baud rate is within 2% of the desired one.
 * @note    Waits for TX to drain and masks UART1_IRQ during the change.
 *          Switch the RAK3172 first (AT+BAUD), otherwise the link breaks.
 */
bool UART1_ChangeBaud(const uint32_t baud);

/**
 * @brief   Writes bytes.
 * @param   data        Write content.
 * @param   length      Number of bytes to write.
 * @note    Blocks while the 32B TX FIFO is full, about 87us per byte beyond the first 32 at 115.2 KBd.
 */
void UART1_Write(const uint8_t *data, const uint16_t length);

/**
 * @brief   Reads one byte from the RX ring buffer.
 * @param   data        Address of outputted byte.
 * @return  TRUE if a byte was read,
 *          FALSE if the buffer is empty.
 * @note    Single consumer: call UART1_Read() and UART1_Flush() from the core that ran UART1_Initialize().
 */
bool UART1_Read(uint8_t *data);

/**
 * @brief   Checks if bytes were dropped because the RX ring buffer (UART1_RX_BUFFER) was full.
 * @return  TRUE if bytes were dropped since the last UART1_Flush(),
 *          FALSE otherwise.
 */
bool UART1_IsOverflowed(void);

/**
 * @brief   Discards every byte in the RX ring buffer and clears the overflow flag.
 */
void UART1_Flush(void);

#endif // UART1_H