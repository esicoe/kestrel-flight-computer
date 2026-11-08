/**
 * @file    console.h
 * @brief   USB serial console.
 * @author  esicoe
 */

#ifndef CONSOLE_H
#define CONSOLE_H

#include "config.h"
#include "pins.h"

#include "led.h"
#include "pyro.h"
#include "lsm6dsv32x.h"
#include "bmp585.h"
#include "w25q128.h"
#include "rak3172.h"
#include "maxm10s.h"
#include "sdcard.h"

#define CONSOLE_PROMPT                              "> "            // Prompt
#define CONSOLE_SEPARATORS                          " \t"           // Word separators
#define CONSOLE_HELP                                "help"          // Command list
#define CONSOLE_STATUS                              "status"        // Component states
#define CONSOLE_INIT                                "init"          // Initialize action
#define CONSOLE_INITIALIZE                          "initialize"    // Initialize action (long form)
#define CONSOLE_DELETE                              0x7F            // Backspace on most terminals
#define CONSOLE_HEX_ROW                             16U             // Hex dump 16B per line
#define CONSOLE_CONNECT_MS                          50U             // Banner delay after USB connect

typedef enum {
    CONSOLE_NONE    = 0,
    CONSOLE_OK      = 1,
    CONSOLE_FAIL    = 2
} console_status_t;

typedef struct {
    const char  *component;
    const char  *action;
    const char  *arguments;
    const char  *description;
    uint8_t     count;
    bool        (*handler)(const char *argument[]);
} console_command_t;

/**
 * @brief   Resets the console state.
 * @note    Call after stdio_init_all(). The banner and prompt are printed when a USB host connects.
 */
void CONSOLE_Initialize(void);

/**
 * @brief   Prints the banner on a USB connect, reads the received characters,
 *          runs the command when a line is complete.
 *          Commands: help, status, then <component> <action> [arguments], e.g. "imu init" or "imu print".
 *          Each command prints its output, then OK or FAIL.
 * @note    Non-blocking until a line is complete, then blocks for the driver call:
 *          lora init about 2s (13s without a module), sd init 1.5s without a card,
 *          flash init and flash erasechip up to 200s.
 *          A component must be initialized ("<component> init") before its other actions.
 *          Numbers are decimal or 0x hex, leading zeros are rejected.
 *          "pyro fire" must be the line right after "pyro arm": any other line disarms.
 *          Lines longer than CONSOLE_BUFFER - 1 characters are rejected.
 */
void CONSOLE_Update(void);

#endif // CONSOLE_H