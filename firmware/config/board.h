/**
 * @file    kestrel.h
 * @brief   MCU compiler settings.
 * @author  esicoe
 */

#ifndef _BOARDS_SYSTEM_H
#define _BOARDS_SYSTEM_H

pico_board_cmake_set(PICO_PLATFORM, rp2350)

#define PICO_RP2350A 0

pico_board_cmake_set_default(PICO_FLASH_SIZE_BYTES, (2 * 1024 * 1024))

#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES (2 * 1024 * 1024)
#endif // PICO_FLASH_SIZE_BYTES

#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 64

#endif // _BOARDS_SYSTEM_H