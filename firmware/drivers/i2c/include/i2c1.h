/**
 * @file    i2c1.h
 * @brief   I2C1 driver.
 * @author  esicoe
 */

#ifndef I2C1_H
#define I2C1_H

#include "config.h"
#include "pins.h"

/**
 * @brief   Initializes I2C1.
 *          Sets bus clock.
 *          Sets GPIO function.
 *          Disables internal pulls.
 * @return  TRUE/FALSE if the desired clock was applied.
 */
bool I2C1_Initialize(void);

/**
 * @brief   Writes to a register.
 * @param   address     Device I2C address.
 * @param   reg         Register address.
 * @param   data        Write content.
 * @return  TRUE on success.
 *          FALSE on failure.
 */
bool I2C1_Write(uint8_t address, uint8_t reg, uint8_t data);

/**
 * @brief   Reads multiple registers in sequence.
 * @param   address     Device I2C address.
 * @param   reg         Register address,
 * @param   data        Address of outputted data.
 * @param   length      Number of registers to read,
 * @return  TRUE on success.
 *          FALSE on failure.
 */
bool I2C1_Read(uint8_t address, uint8_t reg, uint8_t *data, int length);

#endif // I2C1_H
