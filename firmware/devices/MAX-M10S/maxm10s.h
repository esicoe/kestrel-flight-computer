/**
 * @file    maxm10s.h
 * @brief   u-blox MAX-M10S GNSS header.
 * @author  esicoe
 */

#ifndef MAXM10S_H
#define MAXM10S_H

#include "config.h"
#include "pins.h"

#include "uart0.h"

#define VALUE_GNSS_LINE_SIZE                        120U        // NMEA line 82B max + margin
#define VALUE_GNSS_RESET_MS                         10U         // Reset pulse 10ms (datasheet min 1ms)
#define VALUE_GNSS_BOOT_MS                          1000U       // Boot to first NMEA output 1s
#define VALUE_GNSS_READ_MS                          2000U       // Read line timeout 2s (NMEA every 1s at default rate)

/**
 * @brief   Resets the module, lets it boot, flushes any startup noise.
 * @note    Call after UART0_Initialize().
 *          GNSS_RST is driven low for the reset pulse, then released to the internal pull-up (open-drain).
 * @return  Always TRUE: the MAX-M10S has no out-of-band acknowledgement to a reset. Call MAXM10S_ReadLine()
 *          to confirm it is talking.
 */
bool MAXM10S_Configure(void);

/**
 * @brief   Reads one complete NMEA line ("$...\r\n") into a null-terminated buffer.
 * @param   line        Output buffer.
 * @param   size        Size of the output buffer in bytes, 2 or more (room for one character + '\0').
 * @param   timeout_ms  Maximum time to wait for a complete line.
 * @return  TRUE if a full line was read within the timeout,
 *          FALSE on timeout, overflow, or an invalid size argument.
 * @note    Blocks up to timeout_ms. CR and LF are stripped. Lines longer than (size - 1) are dropped.
 *          A UART0 RX buffer overflow causes the current line to be dropped.
 */
bool MAXM10S_ReadLine(char *line, const uint16_t size, const uint32_t timeout_ms);

#endif // MAXM10S_H
