/**
 * @file    rak3172.h
 * @brief   RAK3172 header.
 * @author  esicoe
 */

#ifndef RAK3172_H
#define RAK3172_H

#include "config.h"
#include "pins.h"

#include "uart1.h"
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

#define VALUE_LORA_OK                               "OK"        // Response success
#define VALUE_LORA_ERROR                            "AT_"       // Response error prefix
#define VALUE_LORA_END                              "\r\n"      // Line end CR LF
#define VALUE_LORA_QUERY                            "?"         // Query suffix
#define VALUE_LORA_NWM_P2P                          "0"         // Mode LoRa P2P
#define VALUE_LORA_LPM_OFF                          "0"         // Low-power off
#define VALUE_LORA_PRECV_RX_TX                      "65533"     // RX continuous with TX
#define VALUE_LORA_FREQUENCY                        "915000000" // Frequency 915MHz (902-928MHz band)
#define VALUE_LORA_SF_5                             "5"         // Spreading factor 5
#define VALUE_LORA_SF_6                             "6"         // Spreading factor 6
#define VALUE_LORA_SF_7                             "7"         // Spreading factor 7
#define VALUE_LORA_SF_8                             "8"         // Spreading factor 8
#define VALUE_LORA_SF_9                             "9"         // Spreading factor 9
#define VALUE_LORA_SF_10                            "10"        // Spreading factor 10
#define VALUE_LORA_SF_11                            "11"        // Spreading factor 11
#define VALUE_LORA_SF_12                            "12"        // Spreading factor 12
#define VALUE_LORA_BW_125                           "0"         // Bandwidth 125kHz (index)
#define VALUE_LORA_BW_250                           "1"         // Bandwidth 250kHz (index)
#define VALUE_LORA_BW_500                           "2"         // Bandwidth 500kHz (index)
#define VALUE_LORA_CR_4_5                           "0"         // Coding rate 4/5
#define VALUE_LORA_CR_4_6                           "1"         // Coding rate 4/6
#define VALUE_LORA_CR_4_7                           "2"         // Coding rate 4/7
#define VALUE_LORA_CR_4_8                           "3"         // Coding rate 4/8
#define VALUE_LORA_PREAMBLE                         "8"         // Preamble 8 symbols
#define VALUE_LORA_TX_POWER                         "14"        // Power 14dBm (5-22dBm)

#define VALUE_LORA_PAYLOAD_SIZE                     250U        // Payload 250B max
#define VALUE_LORA_LINE_SIZE                        544U        // Line 544B (250B RX event)
#define VALUE_LORA_RESET_MS                         10U         // Reset pulse 10ms
#define VALUE_LORA_BOOT_MS                          1000U       // Boot or mode restart 1s
#define VALUE_LORA_SYNC_MS                          100U        // AT ping timeout 100ms
#define VALUE_LORA_SYNC_ATTEMPTS                    30U         // AT pings 30 (3s)
#define VALUE_LORA_RESPONSE_MS                      1000U       // Response timeout 1s
#define VALUE_LORA_TX_TIMEOUT_MS                    500U        // TX done timeout 500ms

typedef struct {
    uint64_t    time_us;
    int16_t     rssi_dBm;
    int8_t      snr_dB;
    uint8_t     length;
    uint8_t     payload[VALUE_LORA_PAYLOAD_SIZE];
} rak3172_packet_t;

/**
 * @brief   Resets the module, sets LoRa P2P mode, writes the configuration table,
 *          starts continuous RX that still allows TX (AT+PRECV=65533).
 * @note    Call after UART1_Initialize(). Call every RAK3172 function from that same core.
 *          LORA_RST is driven low for the reset pulse, then released to the 10k pull-up (open-drain).
 *          Needs RUI3 firmware: P2P bandwidth is an index, +EVT:TXP2P DONE exists from RUI3 4.0.5 (check with AT+VER=?).
 *          AT+NWM=0 rewrites the module's NVM and restarts it, so it is only sent when AT+NWM=? reports another mode.
 *          Initialization failure displays LORA_FAIL on LEDs and blocks CPU.
 */
void RAK3172_Initialize(void);

/**
 * @brief   Same as RAK3172_Initialize(), returns the result instead of blocking.
 * @return  TRUE on success,
 *          FALSE on failure.
 */
bool RAK3172_Configure(void);

/**
 * @brief   Sends a P2P packet (AT+PSEND).
 * @param   data        Write content.
 * @param   length      Number of bytes to send, 1 to VALUE_LORA_PAYLOAD_SIZE.
 * @return  TRUE if the module accepted the packet,
 *          FALSE if busy, out of range or no response.
 * @note    Blocks until the module answers OK: about 10-30ms for a 32B packet, up to VALUE_LORA_RESPONSE_MS without an answer.
 *          FALSE after a timeout means the packet may still have been sent. Not callable from an IRQ.
 *          The radio transmits after OK, until RAK3172_IsTransmitting() is FALSE.
 */
bool RAK3172_Send(const uint8_t *data, const uint8_t length);

/**
 * @brief   Processes received events, checks if the last packet is still on air.
 * @return  TRUE until +EVT:TXP2P DONE is received or VALUE_LORA_TX_TIMEOUT_MS has passed,
 *          FALSE otherwise.
 * @note    VALUE_LORA_TX_TIMEOUT_MS covers a 250B packet at SF7/500kHz (97ms on air): raise it for slower settings.
 */
bool RAK3172_IsTransmitting(void);

/**
 * @brief   Processes received events, outputs the last received packet.
 * @param   packet      Address of outputted packet.
 * @return  TRUE if a new packet was received,
 *          FALSE otherwise.
 * @note    Non-blocking. Holds one packet: a newer packet replaces an unread one.
 *          time_us is when the event line was parsed, not the RF reception time.
 *          Lines cut by a UART1 RX overflow are dropped, but the LoRa CRC does not cover the UART link:
 *          protect commands with an application-level CRC.
 */
bool RAK3172_Receive(rak3172_packet_t *packet);

#endif // RAK3172_H