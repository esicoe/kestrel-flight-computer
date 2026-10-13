/**
 * @file    max_m10s_defs.h
 * @brief   MAX-M10S GNSS: UBX sync characters, classes and message IDs.
 * @author  esicoe
 */

#ifndef MAX_M10S_DEFS_H
#define MAX_M10S_DEFS_H

#define OPCODE_GNSS_UBX_SYNC_CHAR_1                 0xB5U
#define OPCODE_GNSS_UBX_SYNC_CHAR_2                 0x62U
#define OPCODE_GNSS_CLASS_NAV                       0x01U
#define OPCODE_GNSS_CLASS_ACK                       0x05U
#define OPCODE_GNSS_CLASS_CFG                       0x06U
#define OPCODE_GNSS_CLASS_MON                       0x0AU
#define OPCODE_GNSS_NAV_STATUS                      0x03U
#define OPCODE_GNSS_NAV_PVT                         0x07U
#define OPCODE_GNSS_NAV_SAT                         0x35U
#define OPCODE_GNSS_ACK_NAK                         0x00U
#define OPCODE_GNSS_ACK_ACK                         0x01U
#define OPCODE_GNSS_CFG_RST                         0x04U
#define OPCODE_GNSS_CFG_VALSET                      0x8AU
#define OPCODE_GNSS_CFG_VALGET                      0x8BU
#define OPCODE_GNSS_CFG_VALDEL                      0x8CU
#define OPCODE_GNSS_MON_VER                         0x04U
#define OPCODE_GNSS_MON_RF                          0x38U

#endif // MAX_M10S_DEFS_H
