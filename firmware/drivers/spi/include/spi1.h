/**
 * @file    spi1.h
 * @brief   SPI1 driver.
 * @author  esicoe
 */

#ifndef SPI1_H
#define SPI1_H

#include "config.h"
#include "pins.h"

/**
 * @brief   Initializes SPI1.
 *          Sets CS as output (idle high),
 *          Sets GPIO function,
 *          Pulls MISO up (SD cards need a pull-up on DAT0, none on the board),
 *          Sets bus clock,
 *          Sets frame format (mode 0, MSB first).
 * @return  TRUE/FALSE if the clock is within 2% of SPI1_CLOCK.
 * @note    400 kHz is not reachable exactly from clk_peri (150 MHz), 398.9 kHz is applied.
 */
bool SPI1_Initialize(void);

/**
 * @brief   Changes the bus clock.
 * @param   clock       Desired clock (Hz).
 * @return  TRUE/FALSE if the clock is within 2% of the desired one.
 * @note    The applied clock is never above the desired one.
 */
bool SPI1_ChangeClock(const uint32_t clock);

/**
 * @brief   Drives CS low.
 */
void SPI1_Select(void);

/**
 * @brief   Drives CS high.
 */
void SPI1_Deselect(void);

/**
 * @brief   Writes one byte and reads the byte clocked in at the same time.
 * @param   data        Write content.
 * @return  Read byte.
 * @note    CS is not driven: wrap transactions in SPI1_Select() and SPI1_Deselect().
 */
uint8_t SPI1_Transfer(const uint8_t data);

/**
 * @brief   Writes bytes.
 * @param   data        Write content.
 * @param   length      Number of bytes to write.
 * @note    CS is not driven: wrap transactions in SPI1_Select() and SPI1_Deselect().
 */
void SPI1_Write(const uint8_t *data, const uint16_t length);

/**
 * @brief   Reads bytes.
 * @param   data        Address of outputted data.
 * @param   length      Number of bytes to read.
 * @note    Sends SPI_DUMMY_BYTE (0xFF) while reading.
 *          CS is not driven: wrap transactions in SPI1_Select() and SPI1_Deselect().
 */
void SPI1_Read(uint8_t *data, const uint16_t length);

#endif // SPI1_H