/**
 * @file    kestrel.h
 * @brief   Pico SDK board definition for the Kestrel flight computer (RP2354B).
 * @author  esicoe
 *
 * Selected in CMakeLists.txt with PICO_BOARD kestrel. Tells the SDK this is the
 * 80-pin RP2350B package (48 GPIOs, so GPIO 32-47 exist) with 2 MB of in-package flash.
 */

#ifndef _BOARDS_KESTREL_H
#define _BOARDS_KESTREL_H

// Chip family for CMake: without this the SDK builds for the RP2040
pico_board_cmake_set(PICO_PLATFORM, rp2350)

// RP2350B package: 48 GPIOs (the pico2 board assumes the 30-GPIO RP2350A)
#define PICO_RP2350A                        0

// RP2354B: 2 MB flash stacked in the package (the cmake line sizes the linker's flash region)
pico_board_cmake_set_default(PICO_FLASH_SIZE_BYTES, (2 * 1024 * 1024))
#ifndef PICO_FLASH_SIZE_BYTES
#define PICO_FLASH_SIZE_BYTES               (2 * 1024 * 1024)
#endif

// 12 MHz ABM8 crystal: give it extra start-up time
#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER  64

#endif // _BOARDS_KESTREL_H
