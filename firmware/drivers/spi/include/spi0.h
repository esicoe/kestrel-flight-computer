/**
 * @file    spi0.h
 * @brief   SPI0 driver.
 * @author  esicoe
 */

#ifndef SPI0_H
#define SPI0_H

#include "config.h"
#include "pins.h"

/**
 * @brief   Initializes SPI0.
 *          Sets CS as output (idle high),
 *          Sets GPIO function,
 *          Sets bus clock,
 *          Sets frame format (mode 0, MSB first).
 * @return  TRUE/FALSE if the desired clock was applied.
 */
bool SPI0_Initialize(void);

/**
 * @brief   Writes bytes in a single transaction.
 * @param   data        Write content.
 * @param   length      Number of bytes to write.
 */
void SPI0_Write(const uint8_t *data, const uint16_t length);

/**
 * @brief   Writes bytes, then reads bytes in a single transaction.
 * @param   writeData   Write content.
 * @param   writeLength Number of bytes to write.
 * @param   readData    Address of outputted data.
 * @param   readLength  Number of bytes to read.
 * @note    Sends SPI_DUMMY_BYTE (0xFF) while reading.
 */
void SPI0_WriteRead(const uint8_t *writeData, const uint16_t writeLength, uint8_t *readData, const uint16_t readLength);

#endif // SPI0_H