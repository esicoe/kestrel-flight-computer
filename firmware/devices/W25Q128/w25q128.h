/**
 * @file    w25q128.h
 * @brief   W25Q128JV header.
 * @author  esicoe
 */

#ifndef W25Q128_H
#define W25Q128_H

#include "config.h"
#include "pins.h"

#include "spi0.h"
#include "led.h"

#define OPCODE_FLASH_WRITE_ENABLE                   0x06U
#define OPCODE_FLASH_VOLATILE_SR_WRITE_ENABLE       0x50U
#define OPCODE_FLASH_WRITE_DISABLE                  0x04U
#define OPCODE_FLASH_RELEASE_POWER_DOWN_ID          0xABU
#define OPCODE_FLASH_MANUFACTURER_DEVICE_ID         0x90U
#define OPCODE_FLASH_JEDEC_ID                       0x9FU
#define OPCODE_FLASH_READ_UNIQUE_ID                 0x4BU
#define OPCODE_FLASH_READ_DATA                      0x03U
#define OPCODE_FLASH_FAST_READ                      0x0BU
#define OPCODE_FLASH_PAGE_PROGRAM                   0x02U
#define OPCODE_FLASH_SECTOR_ERASE_4KB               0x20U
#define OPCODE_FLASH_BLOCK_ERASE_32KB               0x52U
#define OPCODE_FLASH_BLOCK_ERASE_64KB               0xD8U
#define OPCODE_FLASH_CHIP_ERASE                     0xC7U
#define OPCODE_FLASH_READ_STATUS_REGISTER_1         0x05U
#define OPCODE_FLASH_WRITE_STATUS_REGISTER_1        0x01U
#define OPCODE_FLASH_READ_STATUS_REGISTER_2         0x35U
#define OPCODE_FLASH_WRITE_STATUS_REGISTER_2        0x31U
#define OPCODE_FLASH_READ_STATUS_REGISTER_3         0x15U
#define OPCODE_FLASH_WRITE_STATUS_REGISTER_3        0x11U
#define OPCODE_FLASH_READ_SFDP_REGISTER             0x5AU
#define OPCODE_FLASH_ERASE_SECURITY_REGISTER        0x44U
#define OPCODE_FLASH_PROGRAM_SECURITY_REGISTER      0x42U
#define OPCODE_FLASH_READ_SECURITY_REGISTER         0x48U
#define OPCODE_FLASH_GLOBAL_BLOCK_LOCK              0x7EU
#define OPCODE_FLASH_GLOBAL_BLOCK_UNLOCK            0x98U
#define OPCODE_FLASH_READ_BLOCK_LOCK                0x3DU
#define OPCODE_FLASH_INDIVIDUAL_BLOCK_LOCK          0x36U
#define OPCODE_FLASH_INDIVIDUAL_BLOCK_UNLOCK        0x39U
#define OPCODE_FLASH_ERASE_PROGRAM_SUSPEND          0x75U
#define OPCODE_FLASH_ERASE_PROGRAM_RESUME           0x7AU
#define OPCODE_FLASH_POWER_DOWN                     0xB9U
#define OPCODE_FLASH_ENABLE_RESET                   0x66U
#define OPCODE_FLASH_RESET_DEVICE                   0x99U

#define BM_FLASH_SR1_BUSY                           0x01U   // Busy (R)
#define BM_FLASH_SR1_WEL                            0x02U   // Write-enable latch (R)
#define BM_FLASH_SR1_BP                             0x1CU   // Block-protect BP[2:0]
#define BM_FLASH_SR1_TB                             0x20U   // Top/bottom protect
#define BM_FLASH_SR1_SEC                            0x40U   // Sector/block protect
#define BM_FLASH_SR1_SRP                            0x80U   // Status-register protect
#define BM_FLASH_SR2_SRL                            0x01U   // Status-register lock
#define BM_FLASH_SR2_QE                             0x02U   // Quad enable (fixed 1 on -IQ)
#define BM_FLASH_SR2_LB1                            0x08U   // Security-register-1 lock (OTP)
#define BM_FLASH_SR2_LB2                            0x10U   // Security-register-2 lock (OTP)
#define BM_FLASH_SR2_LB3                            0x20U   // Security-register-3 lock (OTP)
#define BM_FLASH_SR2_CMP                            0x40U   // Complement protect
#define BM_FLASH_SR2_SUS                            0x80U   // Suspend status (R)
#define BM_FLASH_SR3_WPS                            0x04U   // Write-protect selection
#define BM_FLASH_SR3_DRV                            0x60U   // Driver-strength DRV[1:0]
#define BM_FLASH_SR3_HOLD_RST                       0x80U   // /HOLD or /RESET select

#define VALUE_FLASH_JEDEC_MANUFACTURER              0xEFU   // Manufacturer Winbond
#define VALUE_FLASH_JEDEC_TYPE                      0x40U   // Type 0x40 (-IQ part)
#define VALUE_FLASH_JEDEC_CAPACITY                  0x18U   // Capacity 128Mbit
#define VALUE_FLASH_DEVICE_ID                       0x17U   // Device 0x17 (ABh/90h)
#define VALUE_FLASH_SR1_BP_NONE                     0x00U   // Protect none (CMP=0)
#define VALUE_FLASH_SR1_BP_ALL                      0x1CU   // Protect all (CMP=0)
#define VALUE_FLASH_SR3_DRV_100                     0x00U   // Drive 100%
#define VALUE_FLASH_SR3_DRV_75                      0x20U   // Drive 75%
#define VALUE_FLASH_SR3_DRV_50                      0x40U   // Drive 50%
#define VALUE_FLASH_SR3_DRV_25                      0x60U   // Drive 25% (-IQ default)

#define VALUE_FLASH_PAGE_SIZE                       0x100U      // Page 256B
#define VALUE_FLASH_SECTOR_SIZE                     0x1000U     // Sector 4KB
#define VALUE_FLASH_BLOCK_32KB_SIZE                 0x8000U     // Block 32KB
#define VALUE_FLASH_BLOCK_64KB_SIZE                 0x10000U    // Block 64KB
#define VALUE_FLASH_MEMORY_SIZE                     0x1000000U  // Memory 16MB

#define VALUE_FLASH_TPUW_MS                         5U      // Power-up to write 5ms min
#define VALUE_FLASH_TRES1_US                        4U      // Release power-down 3us max (+1us margin)
#define VALUE_FLASH_TRES2_US                        3U      // Release power-down + ID 1.8us max (+1us margin)
#define VALUE_FLASH_TDP_US                          4U      // Enter power-down 3us max (+1us margin)
#define VALUE_FLASH_TRST_US                         31U     // Reset 30us max (+1us margin)
#define VALUE_FLASH_TPP_MS                          3U      // Page program 3ms max
#define VALUE_FLASH_TSE_MS                          400U    // Sector erase 400ms max
#define VALUE_FLASH_TBE1_MS                         1600U   // Block-32KB erase 1.6s max
#define VALUE_FLASH_TBE2_MS                         2000U   // Block-64KB erase 2s max
#define VALUE_FLASH_TCE_MS                          200000U // Chip erase 200s max

typedef struct {
    uint8_t     read_opcode;
    uint8_t     write_opcode;
    uint8_t     value;
    uint8_t     mask;
} w25q128_setting_t;

/**
 * @brief   Wakes and resets the chip, checks JEDEC ID, writes configuration table to the status registers.
 *          Status registers are written as volatile bits every boot (no wear, no tW wait):
 *          no block protection, QE kept at 1 (fixed on -IQ), individual block locks off (WPS=0), 25% drive.
 * @note    Call after SPI0_Initialize().
 *          A program/erase left running by an MCU reboot is finished first, up to 200s (tCE).
 *          Initialization failure displays FLASH_FAIL on LEDs and blocks CPU.
 */
void W25Q128_Initialize(void);

/**
 * @brief   Same as W25Q128_Initialize(), returns the result instead of blocking.
 * @return  TRUE on success,
 *          FALSE on failure.
 */
bool W25Q128_Configure(void);

/**
 * @brief   Reads the BUSY bit (program, erase or status write in progress).
 * @return  TRUE if busy,
 *          FALSE if ready.
 */
bool W25Q128_IsBusy(void);

/**
 * @brief   Reads bytes in a single transaction.
 * @param   address     Flash address of the first byte.
 * @param   data        Address of outputted data.
 * @param   length      Number of bytes to read.
 * @return  TRUE on success,
 *          FALSE if busy or out of range.
 * @note    Uses Fast Read (0Bh): one extra dummy byte, but valid at any SPI0_CLOCK (Read Data 03h stops at 50MHz).
 *          A powered-down chip returns zeros: call W25Q128_ReleasePowerDown() first.
 */
bool W25Q128_Read(const uint32_t address, uint8_t *data, const uint16_t length);

/**
 * @brief   Writes bytes, split into page programs at the 256B page boundaries.
 * @param   address     Flash address of the first byte.
 * @param   data        Write content.
 * @param   length      Number of bytes to write.
 * @return  TRUE on success,
 *          FALSE on failure.
 * @note    Programming only clears bits (1 to 0): erase the area first.
 *          Blocks until done, up to 3ms per page (tPP).
 */
bool W25Q128_Write(const uint32_t address, const uint8_t *data, const uint16_t length);

/**
 * @brief   Erases a 4KB sector to 0xFF.
 * @param   address     Sector address, multiple of VALUE_FLASH_SECTOR_SIZE.
 * @return  TRUE on success,
 *          FALSE on failure.
 * @note    Blocks until done, up to 400ms (tSE).
 */
bool W25Q128_EraseSector(const uint32_t address);

/**
 * @brief   Erases a 32KB block to 0xFF.
 * @param   address     Block address, multiple of VALUE_FLASH_BLOCK_32KB_SIZE.
 * @return  TRUE on success,
 *          FALSE on failure.
 * @note    Blocks until done, up to 1.6s (tBE1).
 */
bool W25Q128_EraseBlock32(const uint32_t address);

/**
 * @brief   Erases a 64KB block to 0xFF.
 * @param   address     Block address, multiple of VALUE_FLASH_BLOCK_64KB_SIZE.
 * @return  TRUE on success,
 *          FALSE on failure.
 * @note    Blocks until done, up to 2s (tBE2).
 */
bool W25Q128_EraseBlock64(const uint32_t address);

/**
 * @brief   Erases the whole chip to 0xFF.
 * @return  TRUE on success,
 *          FALSE on failure.
 * @note    Blocks until done, typically 40s, up to 200s (tCE).
 *          Longer than the RP2350 watchdog limit (16.7s): with the watchdog running,
 *          erase with W25Q128_EraseBlock64() in a loop and feed the watchdog between blocks.
 */
bool W25Q128_EraseChip(void);

/**
 * @brief   Enters power-down, the chip then ignores every instruction except release.
 * @return  TRUE on success,
 *          FALSE if busy.
 * @note    Call W25Q128_ReleasePowerDown() before any other W25Q128 function.
 */
bool W25Q128_PowerDown(void);

/**
 * @brief   Leaves power-down and reads the device ID.
 * @return  TRUE if the device ID matches,
 *          FALSE otherwise.
 */
bool W25Q128_ReleasePowerDown(void);

#endif // W25Q128_H