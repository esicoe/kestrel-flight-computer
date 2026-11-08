/**
 * @file    sdcard.h
 * @brief   microSD card header (SPI mode, 512B blocks).
 * @author  esicoe
 */

#ifndef SDCARD_H
#define SDCARD_H

#include "config.h"
#include "pins.h"

#include "spi1.h"
#include "led.h"

#define OPCODE_SD_GO_IDLE_STATE                     0U
#define OPCODE_SD_SEND_IF_COND                      8U
#define OPCODE_SD_SEND_CSD                          9U
#define OPCODE_SD_SEND_CID                          10U
#define OPCODE_SD_STOP_TRANSMISSION                 12U
#define OPCODE_SD_SEND_STATUS                       13U
#define OPCODE_SD_SET_BLOCKLEN                      16U
#define OPCODE_SD_READ_SINGLE_BLOCK                 17U
#define OPCODE_SD_READ_MULTIPLE_BLOCK               18U
#define OPCODE_SD_WRITE_BLOCK                       24U
#define OPCODE_SD_WRITE_MULTIPLE_BLOCK              25U
#define OPCODE_SD_ERASE_WR_BLK_START_ADDR           32U
#define OPCODE_SD_ERASE_WR_BLK_END_ADDR             33U
#define OPCODE_SD_ERASE                             38U
#define OPCODE_SD_APP_CMD                           55U
#define OPCODE_SD_READ_OCR                          58U
#define OPCODE_SD_CRC_ON_OFF                        59U
#define OPCODE_SD_APP_SD_STATUS                     13U
#define OPCODE_SD_APP_SET_WR_BLK_ERASE_COUNT        23U
#define OPCODE_SD_APP_SD_SEND_OP_COND               41U
#define OPCODE_SD_APP_SEND_SCR                      51U
#define OPCODE_SD_START_BLOCK_TOKEN                 0xFEU
#define OPCODE_SD_START_MULTI_WRITE_TOKEN           0xFCU
#define OPCODE_SD_STOP_TRAN_TOKEN                   0xFDU

#define BM_SD_COMMAND_START                         0x40U   // Start + transmission bits
#define BM_SD_CRC_END                               0x01U   // CRC7 end bit
#define BM_SD_R1_IDLE                               0x01U   // In idle state
#define BM_SD_R1_ERASE_RESET                        0x02U   // Erase reset
#define BM_SD_R1_ILLEGAL_COMMAND                    0x04U   // Illegal command
#define BM_SD_R1_CRC_ERROR                          0x08U   // Command CRC error
#define BM_SD_R1_ERASE_SEQUENCE                     0x10U   // Erase sequence error
#define BM_SD_R1_ADDRESS_ERROR                      0x20U   // Address error
#define BM_SD_R1_PARAMETER_ERROR                    0x40U   // Parameter error
#define BM_SD_R1_START                              0x80U   // Start bit (0 in a response)
#define BM_SD_DATA_RESPONSE                         0x1FU   // Data response token

#define BM_SD_ACMD41_HCS                            0x40000000U // Host supports SDHC/SDXC
#define BM_SD_OCR_CCS                               0x40000000U // Card capacity status (block addressing)
#define BM_SD_R7_ECHO                               0x00000FFFU // Voltage accepted + check pattern

#define VALUE_SD_CMD8_ARGUMENT                      0x000001AAU // Voltage 2.7-3.6V pattern 0xAA
#define VALUE_SD_R1_READY                           0x00U       // Ready no error
#define VALUE_SD_R2_OK                              0x00U       // Status no error
#define VALUE_SD_DATA_ACCEPTED                      0x05U       // Data accepted

#define VALUE_SD_BLOCK_SIZE                         512U        // Block 512B
#define VALUE_SD_SDSC_BLOCK_LIMIT                   0x800000U   // SDSC limit 4GB (byte addressing)
#define VALUE_SD_CLOCK                              25000000U   // Clock 25MHz max (default speed)
#define VALUE_SD_POWER_UP_BYTES                     10U         // Power-up 80 clocks (74 min)
#define VALUE_SD_RESPONSE_BYTES                     10U         // Response within 8 bytes (NCR)
#define VALUE_SD_INIT_MS                            1500U       // Initialization 1.5s (spec over 1s)
#define VALUE_SD_READ_MS                            100U        // Read 100ms max
#define VALUE_SD_WRITE_MS                           750U        // Write busy 750ms (spec 500ms)

/**
 * @brief   Puts the card in SPI mode and initializes it:
 *          80 clocks with CS high, CMD0 (idle), CMD8 (voltage check), ACMD41 (wait ready),
 *          CMD58 (addressing mode), CMD16 (512B blocks, SDSC only), then raises SPI1 to VALUE_SD_CLOCK.
 * @note    Call after SPI1_Initialize(). Restores SPI1_CLOCK first, so it can be called again for a reseated card.
 *          Every command first waits up to VALUE_SD_WRITE_MS for a card still busy from before a warm reset,
 *          and CMD0 is retried for VALUE_SD_INIT_MS.
 *          Initialization failure (no card, unsupported card) displays MicroSD_FAIL on LEDs and blocks CPU.
 */
void SDCARD_Initialize(void);

/**
 * @brief   Same as SDCARD_Initialize(), returns the result instead of blocking.
 * @return  TRUE on success,
 *          FALSE on failure.
 */
bool SDCARD_Configure(void);

/**
 * @brief   Reads one block and checks its CRC16.
 * @param   block       Block number (512B units).
 * @param   data        Address of outputted data, VALUE_SD_BLOCK_SIZE bytes.
 * @return  TRUE on success,
 *          FALSE on failure, CRC16 mismatch or block out of range.
 * @note    Blocks up to VALUE_SD_WRITE_MS (card busy) + VALUE_SD_READ_MS (data token).
 */
bool SDCARD_ReadBlock(const uint32_t block, uint8_t *data);

/**
 * @brief   Writes one block, then reads the card status (CMD13) to check the programming result.
 *          CMD13 also runs after a rejected block, clearing the card's latched error bits.
 * @param   block       Block number (512B units).
 * @param   data        Write content, VALUE_SD_BLOCK_SIZE bytes.
 * @return  TRUE on success,
 *          FALSE on failure or block out of range.
 * @note    Blocks until the card finishes programming: typically a few ms, up to 3 x VALUE_SD_WRITE_MS.
 *          Raw blocks, no file system: a FAT library (e.g. FatFs) can sit on top of these two functions.
 */
bool SDCARD_WriteBlock(const uint32_t block, const uint8_t *data);

#endif // SDCARD_H