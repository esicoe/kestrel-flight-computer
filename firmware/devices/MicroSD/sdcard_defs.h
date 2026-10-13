/**
 * @file    sdcard_defs.h
 * @brief   microSD card (SPI mode): commands and data tokens.
 * @author  esicoe
 */

#ifndef SDCARD_DEFS_H
#define SDCARD_DEFS_H

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

#endif // SDCARD_DEFS_H
