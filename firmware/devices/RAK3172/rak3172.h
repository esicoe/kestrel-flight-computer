/**
 * @file    rak3172.h
 * @brief   RAK3172 header.
 * @author  esicoe
 */

#ifndef RAK3172_H
#define RAK3172_H

#include "config.h"
#include "pins.h"

#include "i2c1.h"
#include "led.h"

#define OPCODE_LORA_AT                              "AT"
#define OPCODE_LORA_FIRMWARE_VERSION                "AT+VER=?"
#define OPCODE_LORA_NETWORK_MODE                    "AT+NWM="
#define OPCODE_LORA_P2P_PARAMETERS                  "AT+P2P="
#define OPCODE_LORA_P2P_SEND                        "AT+PSEND="
#define OPCODE_LORA_P2P_RECEIVE                     "AT+PRECV="
#define OPCODE_LORA_LOW_POWER_MODE                  "AT+LPM="
#define OPCODE_LORA_BAND                            "AT+BAND="
#define OPCODE_LORA_EVENT_TX_DONE                   "+EVT:TXP2P DONE"
#define OPCODE_LORA_EVENT_RX                        "+EVT:RXP2P"

#endif // RAK3172_H