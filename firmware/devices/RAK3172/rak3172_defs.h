/**
 * @file    rak3172_defs.h
 * @brief   RAK3172 LoRa module: AT commands and events.
 * @author  esicoe
 */

#ifndef RAK3172_DEFS_H
#define RAK3172_DEFS_H

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

#endif // RAK3172_DEFS_H
