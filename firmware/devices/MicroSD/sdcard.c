#include "sdcard.h"

static bool highCapacity = false;

static uint8_t SDCARD_Crc7(const uint8_t *data, const uint8_t length) {
    uint8_t crc = 0;

    for (uint8_t i = 0; i < length; i++) {
        crc = crc ^ data[i];

        for (uint8_t j = 0; j < 8; j++) {
            if (crc >> 7) {
                crc = (crc << 1) ^ 0x12;
            }
            else {
                crc = crc << 1;
            }
        }
    }

    return crc | BM_SD_CRC_END;
}

static bool SDCARD_WaitReady(const uint32_t timeout) {
    uint64_t deadline = time_us_64() + ((uint64_t)timeout * 1000);

    while (SPI1_Transfer(SPI_DUMMY_BYTE) != SPI_DUMMY_BYTE) {
        if (time_us_64() > deadline) {
            return (SPI1_Transfer(SPI_DUMMY_BYTE) == SPI_DUMMY_BYTE);
        }
    }

    return true;
}

static void SDCARD_Release(void) {
    SPI1_Deselect();
    SPI1_Transfer(SPI_DUMMY_BYTE);
}

static uint8_t SDCARD_Command(const uint8_t command, const uint32_t argument) {
    uint8_t frame[6];
    uint8_t response = SPI_DUMMY_BYTE;

    frame[0] = BM_SD_COMMAND_START | command;
    frame[1] = (uint8_t)(argument >> 24);
    frame[2] = (uint8_t)(argument >> 16);
    frame[3] = (uint8_t)(argument >> 8);
    frame[4] = (uint8_t)argument;
    frame[5] = SDCARD_Crc7(frame, 5);

    SPI1_Select();
    SPI1_Transfer(SPI_DUMMY_BYTE);

    if (!SDCARD_WaitReady(VALUE_SD_WRITE_MS)) {
        return response;
    }

    SPI1_Write(frame, 6);

    for (uint8_t i = 0; i < VALUE_SD_RESPONSE_BYTES; i++) {
        response = SPI1_Transfer(SPI_DUMMY_BYTE);

        if ((response & BM_SD_R1_START) == 0) {
            return response;
        }
    }

    return response;
}

static uint8_t SDCARD_AppCommand(const uint8_t command, const uint32_t argument) {
    uint8_t response = SDCARD_Command(OPCODE_SD_APP_CMD, 0);

    SDCARD_Release();

    if (response > BM_SD_R1_IDLE) {
        return response;
    }

    return SDCARD_Command(command, argument);
}

static uint32_t SDCARD_ReadPayload(void) {
    uint8_t buffer[4];

    SPI1_Read(buffer, 4);

    return ((uint32_t)buffer[0] << 24) | ((uint32_t)buffer[1] << 16) | ((uint32_t)buffer[2] << 8) | buffer[3];
}

static bool SDCARD_GoIdle(void) {
    uint64_t deadline = time_us_64() + ((uint64_t)VALUE_SD_INIT_MS * 1000);
    uint8_t response;

    do {
        response = SDCARD_Command(OPCODE_SD_GO_IDLE_STATE, 0);
        SDCARD_Release();
    } while ((response != BM_SD_R1_IDLE) && (time_us_64() < deadline));

    return (response == BM_SD_R1_IDLE);
}

void SDCARD_Initialize(void) {
    if (!SDCARD_Configure()) {
        LED_DisplayError(MicroSD_FAIL);
    }
}

bool SDCARD_Configure(void) {
    bool status = true;
    uint8_t response;
    uint32_t payload;
    uint32_t argument = 0;
    uint64_t deadline;

    status &= SPI1_ChangeClock(SPI1_CLOCK);

    SPI1_Deselect();

    for (uint8_t i = 0; i < VALUE_SD_POWER_UP_BYTES; i++) {
        SPI1_Transfer(SPI_DUMMY_BYTE);
    }

    status &= SDCARD_GoIdle();

    response = SDCARD_Command(OPCODE_SD_SEND_IF_COND, VALUE_SD_CMD8_ARGUMENT);
    payload = SDCARD_ReadPayload();
    SDCARD_Release();

    if (response == BM_SD_R1_IDLE) {
        status &= ((payload & BM_SD_R7_ECHO) == VALUE_SD_CMD8_ARGUMENT);
        argument = BM_SD_ACMD41_HCS;
    }
    else {
        status &= (response == (BM_SD_R1_IDLE | BM_SD_R1_ILLEGAL_COMMAND));
    }

    deadline = time_us_64() + ((uint64_t)VALUE_SD_INIT_MS * 1000);

    do {
        response = SDCARD_AppCommand(OPCODE_SD_APP_SD_SEND_OP_COND, argument);
        SDCARD_Release();
    } while ((response == BM_SD_R1_IDLE) && (time_us_64() < deadline));

    status &= (response == VALUE_SD_R1_READY);

    response = SDCARD_Command(OPCODE_SD_READ_OCR, 0);
    payload = SDCARD_ReadPayload();
    SDCARD_Release();

    status &= (response == VALUE_SD_R1_READY);
    highCapacity = ((payload & BM_SD_OCR_CCS) != 0);

    if (!highCapacity) {
        response = SDCARD_Command(OPCODE_SD_SET_BLOCKLEN, VALUE_SD_BLOCK_SIZE);
        SDCARD_Release();

        status &= (response == VALUE_SD_R1_READY);
    }

    status &= SPI1_ChangeClock(VALUE_SD_CLOCK);

    return status;
}

static bool SDCARD_BlockAddress(const uint32_t block, uint32_t *address) {
    if (highCapacity) {
        *address = block;

        return true;
    }

    *address = block * VALUE_SD_BLOCK_SIZE;

    return (block < VALUE_SD_SDSC_BLOCK_LIMIT);
}

static bool SDCARD_WaitToken(const uint8_t token, const uint32_t timeout) {
    uint64_t deadline = time_us_64() + ((uint64_t)timeout * 1000);
    uint8_t response;

    do {
        response = SPI1_Transfer(SPI_DUMMY_BYTE);
    } while ((response == SPI_DUMMY_BYTE) && (time_us_64() < deadline));

    return (response == token);
}

static uint16_t SDCARD_Crc16(const uint8_t *data, const uint16_t length) {
    uint16_t crc = 0;

    for (uint16_t i = 0; i < length; i++) {
        crc = crc ^ ((uint16_t)data[i] << 8);

        for (uint8_t j = 0; j < 8; j++) {
            if (crc >> 15) {
                crc = (crc << 1) ^ 0x1021;
            }
            else {
                crc = crc << 1;
            }
        }
    }

    return crc;
}

bool SDCARD_ReadBlock(const uint32_t block, uint8_t *data) {
    uint8_t crc[2];
    uint32_t address;
    bool status;

    status = SDCARD_BlockAddress(block, &address);
    status = status && (SDCARD_Command(OPCODE_SD_READ_SINGLE_BLOCK, address) == VALUE_SD_R1_READY);
    status = status && SDCARD_WaitToken(OPCODE_SD_START_BLOCK_TOKEN, VALUE_SD_READ_MS);

    if (status) {
        SPI1_Read(data, VALUE_SD_BLOCK_SIZE);
        SPI1_Read(crc, 2);

        status = (SDCARD_Crc16(data, VALUE_SD_BLOCK_SIZE) == (((uint16_t)crc[0] << 8) | crc[1]));
    }

    SDCARD_Release();

    return status;
}

bool SDCARD_WriteBlock(const uint32_t block, const uint8_t *data) {
    uint32_t address;
    bool started;
    bool status;

    started = SDCARD_BlockAddress(block, &address);
    started = started && (SDCARD_Command(OPCODE_SD_WRITE_BLOCK, address) == VALUE_SD_R1_READY);
    status = started;

    if (started) {
        SPI1_Transfer(SPI_DUMMY_BYTE);
        SPI1_Transfer(OPCODE_SD_START_BLOCK_TOKEN);
        SPI1_Write(data, VALUE_SD_BLOCK_SIZE);
        SPI1_Transfer(SPI_DUMMY_BYTE);
        SPI1_Transfer(SPI_DUMMY_BYTE);

        status = ((SPI1_Transfer(SPI_DUMMY_BYTE) & BM_SD_DATA_RESPONSE) == VALUE_SD_DATA_ACCEPTED);
        status &= SDCARD_WaitReady(VALUE_SD_WRITE_MS);
    }

    SDCARD_Release();

    if (started) {
        status &= (SDCARD_Command(OPCODE_SD_SEND_STATUS, 0) == VALUE_SD_R1_READY);
        status &= (SPI1_Transfer(SPI_DUMMY_BYTE) == VALUE_SD_R2_OK);

        SDCARD_Release();
    }

    return status;
}