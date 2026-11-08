#include "w25q128.h"

static const w25q128_setting_t w25q128Config[] = {
    {OPCODE_FLASH_READ_STATUS_REGISTER_1, OPCODE_FLASH_WRITE_STATUS_REGISTER_1, VALUE_FLASH_SR1_BP_NONE, BM_FLASH_SR1_SRP | BM_FLASH_SR1_SEC | BM_FLASH_SR1_TB | BM_FLASH_SR1_BP},
    {OPCODE_FLASH_READ_STATUS_REGISTER_2, OPCODE_FLASH_WRITE_STATUS_REGISTER_2, BM_FLASH_SR2_QE,         BM_FLASH_SR2_CMP | BM_FLASH_SR2_QE  | BM_FLASH_SR2_SRL                 },
    {OPCODE_FLASH_READ_STATUS_REGISTER_3, OPCODE_FLASH_WRITE_STATUS_REGISTER_3, VALUE_FLASH_SR3_DRV_25,  BM_FLASH_SR3_DRV | BM_FLASH_SR3_WPS                                    },
};

static uint8_t W25Q128_ReadStatusRegister(const uint8_t opcode) {
    uint8_t output;

    SPI0_WriteRead((uint8_t[]){opcode}, 1, &output, 1);

    return output;
}

bool W25Q128_IsBusy(void) {
    uint8_t output = W25Q128_ReadStatusRegister(OPCODE_FLASH_READ_STATUS_REGISTER_1);

    return ((output & BM_FLASH_SR1_BUSY) != 0);
}

static bool W25Q128_WaitUntilReady(const uint32_t timeout) {
    uint64_t deadline = time_us_64() + ((uint64_t)timeout * 1000);

    while (W25Q128_IsBusy()) {
        if (time_us_64() > deadline) {
            return !W25Q128_IsBusy();
        }
    }

    return true;
}

void W25Q128_Initialize(void) {
    if (!W25Q128_Configure()) {
        LED_DisplayError(FLASH_FAIL);
    }
}

bool W25Q128_Configure(void) {
    bool status = true;
    uint8_t jedec[3] = {0};
    uint8_t output;

    sleep_ms(VALUE_FLASH_TPUW_MS);

    SPI0_Write((uint8_t[]){OPCODE_FLASH_RELEASE_POWER_DOWN_ID}, 1);
    sleep_us(VALUE_FLASH_TRES1_US);

    status &= W25Q128_WaitUntilReady(VALUE_FLASH_TCE_MS);

    SPI0_Write((uint8_t[]){OPCODE_FLASH_ENABLE_RESET}, 1);
    SPI0_Write((uint8_t[]){OPCODE_FLASH_RESET_DEVICE}, 1);
    sleep_us(VALUE_FLASH_TRST_US);

    SPI0_WriteRead((uint8_t[]){OPCODE_FLASH_JEDEC_ID}, 1, jedec, 3);

    status &= (jedec[0] == VALUE_FLASH_JEDEC_MANUFACTURER);
    status &= (jedec[1] == VALUE_FLASH_JEDEC_TYPE);
    status &= (jedec[2] == VALUE_FLASH_JEDEC_CAPACITY);

    for (uint8_t i = 0; i < (sizeof(w25q128Config) / sizeof(w25q128Config[0])); i++) {
        SPI0_Write((uint8_t[]){OPCODE_FLASH_VOLATILE_SR_WRITE_ENABLE}, 1);
        SPI0_Write((uint8_t[]){w25q128Config[i].write_opcode, w25q128Config[i].value}, 2);
    }

    for (uint8_t i = 0; i < (sizeof(w25q128Config) / sizeof(w25q128Config[0])); i++) {
        output = W25Q128_ReadStatusRegister(w25q128Config[i].read_opcode);
        status &= ((output & w25q128Config[i].mask) == w25q128Config[i].value);
    }

    return status;
}

static bool W25Q128_IsInRange(const uint32_t address, const uint32_t length) {
    return (((uint64_t)address + length) <= VALUE_FLASH_MEMORY_SIZE);
}

bool W25Q128_Read(const uint32_t address, uint8_t *data, const uint16_t length) {
    uint8_t command[5] = {OPCODE_FLASH_FAST_READ, (uint8_t)(address >> 16), (uint8_t)(address >> 8), (uint8_t)address, SPI_DUMMY_BYTE};

    if (!W25Q128_IsInRange(address, length) || W25Q128_IsBusy()) {
        return false;
    }

    SPI0_WriteRead(command, 5, data, length);

    return true;
}

static bool W25Q128_WriteEnable(void) {
    uint8_t output;

    SPI0_Write((uint8_t[]){OPCODE_FLASH_WRITE_ENABLE}, 1);

    output = W25Q128_ReadStatusRegister(OPCODE_FLASH_READ_STATUS_REGISTER_1);

    return ((output & (BM_FLASH_SR1_BUSY | BM_FLASH_SR1_WEL)) == BM_FLASH_SR1_WEL);
}

bool W25Q128_Write(const uint32_t address, const uint8_t *data, const uint16_t length) {
    uint8_t buffer[4 + VALUE_FLASH_PAGE_SIZE];
    uint32_t current;
    uint16_t written = 0;
    uint16_t chunk;

    if (!W25Q128_IsInRange(address, length)) {
        return false;
    }

    while (written < length) {
        current = address + written;

        chunk = VALUE_FLASH_PAGE_SIZE - (current % VALUE_FLASH_PAGE_SIZE);
        if (chunk > (length - written)) {
            chunk = length - written;
        }

        if (!W25Q128_WriteEnable()) {
            return false;
        }

        buffer[0] = OPCODE_FLASH_PAGE_PROGRAM;
        buffer[1] = (uint8_t)(current >> 16);
        buffer[2] = (uint8_t)(current >> 8);
        buffer[3] = (uint8_t)current;

        for (uint16_t i = 0; i < chunk; i++) {
            buffer[4 + i] = data[written + i];
        }

        SPI0_Write(buffer, 4 + chunk);

        if (!W25Q128_WaitUntilReady(VALUE_FLASH_TPP_MS)) {
            return false;
        }

        written += chunk;
    }

    return true;
}

static bool W25Q128_Erase(const uint8_t opcode, const uint32_t address, const uint32_t size, const uint32_t timeout) {
    if (((address % size) != 0) || !W25Q128_IsInRange(address, size)) {
        return false;
    }

    if (!W25Q128_WriteEnable()) {
        return false;
    }

    SPI0_Write((uint8_t[]){opcode, (uint8_t)(address >> 16), (uint8_t)(address >> 8), (uint8_t)address}, 4);

    return W25Q128_WaitUntilReady(timeout);
}

bool W25Q128_EraseSector(const uint32_t address) {
    return W25Q128_Erase(OPCODE_FLASH_SECTOR_ERASE_4KB, address, VALUE_FLASH_SECTOR_SIZE, VALUE_FLASH_TSE_MS);
}

bool W25Q128_EraseBlock32(const uint32_t address) {
    return W25Q128_Erase(OPCODE_FLASH_BLOCK_ERASE_32KB, address, VALUE_FLASH_BLOCK_32KB_SIZE, VALUE_FLASH_TBE1_MS);
}

bool W25Q128_EraseBlock64(const uint32_t address) {
    return W25Q128_Erase(OPCODE_FLASH_BLOCK_ERASE_64KB, address, VALUE_FLASH_BLOCK_64KB_SIZE, VALUE_FLASH_TBE2_MS);
}

bool W25Q128_EraseChip(void) {
    if (!W25Q128_WriteEnable()) {
        return false;
    }

    SPI0_Write((uint8_t[]){OPCODE_FLASH_CHIP_ERASE}, 1);

    return W25Q128_WaitUntilReady(VALUE_FLASH_TCE_MS);
}

bool W25Q128_PowerDown(void) {
    if (W25Q128_IsBusy()) {
        return false;
    }

    SPI0_Write((uint8_t[]){OPCODE_FLASH_POWER_DOWN}, 1);
    sleep_us(VALUE_FLASH_TDP_US);

    return true;
}

bool W25Q128_ReleasePowerDown(void) {
    uint8_t output;

    SPI0_WriteRead((uint8_t[]){OPCODE_FLASH_RELEASE_POWER_DOWN_ID, SPI_DUMMY_BYTE, SPI_DUMMY_BYTE, SPI_DUMMY_BYTE}, 4, &output, 1);
    sleep_us(VALUE_FLASH_TRES2_US);

    return (output == VALUE_FLASH_DEVICE_ID);
}