#include "console.h"

static const char *const consoleComponents[] = {"led", "imu", "baro", "flash", "lora", "gnss", "sd", "pyro"};
static const char *const consoleStatusText[] = {"-", "OK", "FAIL"};

static console_status_t consoleStatus[sizeof(consoleComponents) / sizeof(consoleComponents[0])];
static char lineBuffer[CONSOLE_BUFFER];
static uint16_t lineLength = 0;
static bool lineOverflow = false;
static char lastCharacter = 0;
static bool pyroArmed = false;
static bool pyroReady = false;
static bool usbConnected = false;
static bool bannerPending = false;
static uint64_t connectTime = 0;

void CONSOLE_Initialize(void) {
    lineLength = 0;
    lineOverflow = false;
    lastCharacter = 0;
    pyroArmed = false;
    usbConnected = false;
    bannerPending = false;
}

static int8_t CONSOLE_HexValue(const char c) {
    if ((c >= '0') && (c <= '9')) {
        return c - '0';
    }

    if ((c >= 'A') && (c <= 'F')) {
        return c - 'A' + 10;
    }

    if ((c >= 'a') && (c <= 'f')) {
        return c - 'a' + 10;
    }

    return -1;
}

static uint16_t CONSOLE_ParseHex(const char *text, uint8_t *data, const uint16_t size) {
    uint16_t length = 0;
    int8_t high;
    int8_t low;

    while (*text != '\0') {
        high = CONSOLE_HexValue(text[0]);
        low = CONSOLE_HexValue(text[1]);

        if ((high < 0) || (low < 0) || (length >= size)) {
            return 0;
        }

        data[length] = (uint8_t)((high << 4) | low);
        length++;
        text += 2;
    }

    return length;
}

static bool CONSOLE_ParseNumber(const char *text, uint32_t *value) {
    char *end;
    int base = 10;

    if ((text[0] < '0') || (text[0] > '9')) {
        return false;
    }

    if ((text[0] == '0') && ((text[1] == 'x') || (text[1] == 'X'))) {
        base = 16;
    }

    if ((base == 10) && (text[0] == '0') && (text[1] != '\0')) {
        return false;
    }

    errno = 0;
    *value = strtoul(text, &end, base);

    return ((end != text) && (*end == '\0') && (errno == 0));
}

static void CONSOLE_PrintHex(const uint8_t *data, const uint16_t length, const uint32_t address) {
    for (uint16_t i = 0; i < length; i++) {
        if ((i % CONSOLE_HEX_ROW) == 0) {
            printf("%08lX:", (unsigned long)(address + i));
        }

        printf(" %02X", data[i]);

        if (((i % CONSOLE_HEX_ROW) == (CONSOLE_HEX_ROW - 1)) || (i == (length - 1))) {
            printf("\n");
        }
    }
}

static bool CONSOLE_LedInit(const char *argument[]) {
    static bool started = false;

    (void)argument;

    if (!started) {
        LED_Initialize();
        started = true;
    }

    return true;
}

static bool CONSOLE_LedSet(const char *argument[]) {
    uint32_t code;

    if (!CONSOLE_ParseNumber(argument[0], &code) || (code > UINT8_MAX)) {
        return false;
    }

    LED_DisplayStatus((uint8_t)code);

    return true;
}

static bool CONSOLE_ImuInit(const char *argument[]) {
    (void)argument;

    return (I2C0_Initialize() && LSM6DSV32X_Configure());
}

static bool CONSOLE_ImuPrint(const char *argument[]) {
    lsm6dsv32x_output_t output = LSM6DSV32X_Read();

    (void)argument;

    if (output.error) {
        return false;
    }

    printf("time %llu us\n", (unsigned long long)output.time_us);
    printf("gyro %.2f %.2f %.2f dps\n", output.gyro_x_dps, output.gyro_y_dps, output.gyro_z_dps);
    printf("accel %.2f %.2f %.2f m/s2\n", output.accel_x_ms2, output.accel_y_ms2, output.accel_z_ms2);

    return true;
}

static bool CONSOLE_BaroInit(const char *argument[]) {
    (void)argument;

    return (I2C1_Initialize() && BMP585_Configure());
}

static bool CONSOLE_BaroPrint(const char *argument[]) {
    bmp585_output_t output = BMP585_Read();

    (void)argument;

    if (output.error) {
        return false;
    }

    printf("time %llu us\n", (unsigned long long)output.time_us);
    printf("pressure %.1f Pa\n", output.pressure_Pa);
    printf("temperature %u K\n", (unsigned int)output.temperature_K);

    return true;
}

static bool CONSOLE_FlashInit(const char *argument[]) {
    (void)argument;

    return (SPI0_Initialize() && W25Q128_Configure());
}

static bool CONSOLE_FlashRead(const char *argument[]) {
    uint8_t data[VALUE_FLASH_PAGE_SIZE];
    uint32_t address;
    uint32_t length;

    if (!CONSOLE_ParseNumber(argument[0], &address) || !CONSOLE_ParseNumber(argument[1], &length)) {
        return false;
    }

    if ((length == 0) || (length > VALUE_FLASH_PAGE_SIZE) || !W25Q128_Read(address, data, (uint16_t)length)) {
        return false;
    }

    CONSOLE_PrintHex(data, (uint16_t)length, address);

    return true;
}

static bool CONSOLE_FlashWrite(const char *argument[]) {
    uint8_t data[VALUE_FLASH_PAGE_SIZE];
    uint32_t address;
    uint16_t length = CONSOLE_ParseHex(argument[1], data, VALUE_FLASH_PAGE_SIZE);

    if (!CONSOLE_ParseNumber(argument[0], &address) || (length == 0)) {
        return false;
    }

    return W25Q128_Write(address, data, length);
}

static bool CONSOLE_FlashErase(const char *argument[]) {
    uint32_t address;

    if (!CONSOLE_ParseNumber(argument[0], &address)) {
        return false;
    }

    return W25Q128_EraseSector(address);
}

static bool CONSOLE_FlashEraseChip(const char *argument[]) {
    (void)argument;

    printf("erasing, up to 200 s\n");
    stdio_flush();

    return W25Q128_EraseChip();
}

static bool CONSOLE_LoraInit(const char *argument[]) {
    (void)argument;

    return (UART1_Initialize() && RAK3172_Configure());
}

static bool CONSOLE_LoraSend(const char *argument[]) {
    uint8_t data[VALUE_LORA_PAYLOAD_SIZE];
    uint16_t length = CONSOLE_ParseHex(argument[0], data, VALUE_LORA_PAYLOAD_SIZE);

    if (length == 0) {
        return false;
    }

    return RAK3172_Send(data, (uint8_t)length);
}

static bool CONSOLE_LoraReceive(const char *argument[]) {
    rak3172_packet_t packet;

    (void)argument;

    if (!RAK3172_Receive(&packet)) {
        printf("no packet\n");

        return true;
    }

    printf("rssi %d dBm, snr %d dB, %u bytes\n", packet.rssi_dBm, packet.snr_dB, (unsigned int)packet.length);
    CONSOLE_PrintHex(packet.payload, packet.length, 0);

    return true;
}

static bool CONSOLE_GnssInit(const char *argument[]) {
    (void)argument;

    return (UART0_Initialize() && MAXM10S_Configure());
}

static bool CONSOLE_GnssPrint(const char *argument[]) {
    char line[VALUE_GNSS_LINE_SIZE];

    (void)argument;

    if (!MAXM10S_ReadLine(line, VALUE_GNSS_LINE_SIZE, VALUE_GNSS_READ_MS)) {
        printf("no line\n");

        return false;
    }

    printf("%s\n", line);

    return true;
}

static bool CONSOLE_GnssRaw(const char *argument[]) {
    uint64_t deadline = time_us_64() + ((uint64_t)3000 * 1000);
    uint32_t count = 0;
    uint8_t data;

    (void)argument;

    UART0_Flush();

    while (time_us_64() < deadline) {
        if (!UART0_Read(&data)) {
            continue;
        }

        if ((count % CONSOLE_HEX_ROW) == 0) {
            printf("%04lX:", (unsigned long)count);
        }

        printf(" %02X", data);

        if ((count % CONSOLE_HEX_ROW) == (CONSOLE_HEX_ROW - 1)) {
            printf("\n");
        }

        count++;
    }

    if ((count % CONSOLE_HEX_ROW) != 0) {
        printf("\n");
    }

    printf("%lu bytes in 3s\n", (unsigned long)count);

    return true;
}

static bool CONSOLE_SdInit(const char *argument[]) {
    (void)argument;

    return (SPI1_Initialize() && SDCARD_Configure());
}

static bool CONSOLE_SdRead(const char *argument[]) {
    uint8_t data[VALUE_SD_BLOCK_SIZE];
    uint32_t block;

    if (!CONSOLE_ParseNumber(argument[0], &block) || !SDCARD_ReadBlock(block, data)) {
        return false;
    }

    CONSOLE_PrintHex(data, VALUE_SD_BLOCK_SIZE, 0);

    return true;
}

static bool CONSOLE_SdWrite(const char *argument[]) {
    uint8_t data[VALUE_SD_BLOCK_SIZE] = {0};
    uint32_t block;

    if (!CONSOLE_ParseNumber(argument[0], &block) || (CONSOLE_ParseHex(argument[1], data, VALUE_SD_BLOCK_SIZE) == 0)) {
        return false;
    }

    return SDCARD_WriteBlock(block, data);
}

static bool CONSOLE_PyroInit(const char *argument[]) {
    (void)argument;

    PYRO_Initialize();
    pyroArmed = false;

    return true;
}

static bool CONSOLE_PyroArm(const char *argument[]) {
    (void)argument;

    pyroArmed = true;
    printf("armed, the next line must be pyro fire\n");

    return true;
}

static bool CONSOLE_PyroDisarm(const char *argument[]) {
    (void)argument;

    pyroArmed = false;

    return true;
}

static bool CONSOLE_PyroFire(const char *argument[]) {
    uint32_t channel;
    uint32_t duration;

    if (!pyroReady) {
        printf("not armed, run pyro arm first\n");

        return false;
    }

    if (!CONSOLE_ParseNumber(argument[0], &channel) || !CONSOLE_ParseNumber(argument[1], &duration)) {
        return false;
    }

    if ((channel == 0) || (channel > (PYRO_3 + 1))) {
        return false;
    }

    return PYRO_Fire((pyro_channel_t)(channel - 1), duration);
}

static bool CONSOLE_PyroOff(const char *argument[]) {
    uint32_t channel;

    if (!CONSOLE_ParseNumber(argument[0], &channel) || (channel == 0) || (channel > (PYRO_3 + 1))) {
        return false;
    }

    PYRO_Off((pyro_channel_t)(channel - 1));

    return true;
}

static const console_command_t consoleCommands[] = {
    {"led",   "init",      "",                   "Initializes the status LEDs",                     0, CONSOLE_LedInit       },
    {"led",   "set",       "<code>",             "Shows a status code (bits 0-2 solid, 3-5 blink)", 1, CONSOLE_LedSet        },
    {"imu",   "init",      "",                   "Initializes I2C0 and the LSM6DSV32X",             0, CONSOLE_ImuInit       },
    {"imu",   "print",     "",                   "Prints one gyroscope and accelerometer reading",  0, CONSOLE_ImuPrint      },
    {"baro",  "init",      "",                   "Initializes I2C1 and the BMP585",                 0, CONSOLE_BaroInit      },
    {"baro",  "print",     "",                   "Prints one pressure and temperature reading",     0, CONSOLE_BaroPrint     },
    {"flash", "init",      "",                   "Initializes SPI0 and the W25Q128",                0, CONSOLE_FlashInit     },
    {"flash", "read",      "<address> <length>", "Dumps 1 to 256 bytes",                            2, CONSOLE_FlashRead     },
    {"flash", "write",     "<address> <hex>",    "Programs bytes (erase first)",                    2, CONSOLE_FlashWrite    },
    {"flash", "erase",     "<address>",          "Erases the 4KB sector at a 4KB-aligned address",  1, CONSOLE_FlashErase    },
    {"flash", "erasechip", "",                   "Erases the whole chip (up to 200s)",              0, CONSOLE_FlashEraseChip},
    {"lora",  "init",      "",                   "Initializes UART1 and the RAK3172 (P2P, RX on)",  0, CONSOLE_LoraInit      },
    {"lora",  "send",      "<hex>",              "Sends a P2P packet (1 to 250 bytes)",             1, CONSOLE_LoraSend      },
    {"lora",  "receive",   "",                   "Prints the last received packet",                 0, CONSOLE_LoraReceive   },
    {"gnss",  "init",      "",                   "Initializes UART0 and the MAX-M10S",              0, CONSOLE_GnssInit      },
    {"gnss",  "print",     "",                   "Prints one NMEA line (up to 2s)",                 0, CONSOLE_GnssPrint     },
    {"gnss",  "raw",       "",                   "Dumps raw UART0 bytes for 3s",                    0, CONSOLE_GnssRaw       },
    {"sd",    "init",      "",                   "Initializes SPI1 and the microSD card",           0, CONSOLE_SdInit        },
    {"sd",    "read",      "<block>",            "Dumps one 512B block",                            1, CONSOLE_SdRead        },
    {"sd",    "write",     "<block> <hex>",      "Writes bytes to a block, the rest zero",          2, CONSOLE_SdWrite       },
    {"pyro",  "init",      "",                   "Initializes the pyro outputs (all off)",          0, CONSOLE_PyroInit      },
    {"pyro",  "arm",       "",                   "Allows a pyro fire on the next line only",        0, CONSOLE_PyroArm       },
    {"pyro",  "disarm",    "",                   "Cancels pyro arm",                                0, CONSOLE_PyroDisarm    },
    {"pyro",  "fire",      "<channel> <ms>",     "Fires channel 1-3 for ms (line after pyro arm)",  2, CONSOLE_PyroFire      },
    {"pyro",  "off",       "<channel>",          "Turns channel 1-3 off",                           1, CONSOLE_PyroOff       },
};

static void CONSOLE_PrintLine(const char *component, const char *action, const char *arguments, const char *description) {
    printf("%-6s %-10s %-19s %s\n", component, action, arguments, description);
}

static void CONSOLE_PrintHelp(void) {
    CONSOLE_PrintLine(CONSOLE_HELP, "", "", "Lists the commands");
    CONSOLE_PrintLine(CONSOLE_STATUS, "", "", "Lists the component states");

    for (uint8_t i = 0; i < (sizeof(consoleCommands) / sizeof(consoleCommands[0])); i++) {
        CONSOLE_PrintLine(consoleCommands[i].component, consoleCommands[i].action, consoleCommands[i].arguments, consoleCommands[i].description);
    }
}

static void CONSOLE_PrintStatus(void) {
    for (uint8_t i = 0; i < (sizeof(consoleComponents) / sizeof(consoleComponents[0])); i++) {
        printf("%-6s %s\n", consoleComponents[i], consoleStatusText[consoleStatus[i]]);
    }
}

static int8_t CONSOLE_FindComponent(const char *name) {
    for (uint8_t i = 0; i < (sizeof(consoleComponents) / sizeof(consoleComponents[0])); i++) {
        if (strcmp(consoleComponents[i], name) == 0) {
            return (int8_t)i;
        }
    }

    return -1;
}

static const console_command_t *CONSOLE_FindCommand(const char *component, const char *action) {
    for (uint8_t i = 0; i < (sizeof(consoleCommands) / sizeof(consoleCommands[0])); i++) {
        if ((strcmp(consoleCommands[i].component, component) == 0) && (strcmp(consoleCommands[i].action, action) == 0)) {
            return &consoleCommands[i];
        }
    }

    return NULL;
}

static void CONSOLE_Execute(char *line) {
    const char *word[CONSOLE_WORDS];
    const console_command_t *command = NULL;
    char *token;
    uint8_t count = 0;
    int8_t component;
    bool initialize;
    bool status;

    token = strtok(line, CONSOLE_SEPARATORS);

    while ((token != NULL) && (count < CONSOLE_WORDS)) {
        word[count] = token;
        count++;
        token = strtok(NULL, CONSOLE_SEPARATORS);
    }

    if (count == 0) {
        return;
    }

    pyroReady = pyroArmed;
    pyroArmed = false;

    if (token != NULL) {
        printf("FAIL: too many words\n");

        return;
    }

    if ((count == 1) && (strcmp(word[0], CONSOLE_HELP) == 0)) {
        CONSOLE_PrintHelp();

        return;
    }

    if ((count == 1) && (strcmp(word[0], CONSOLE_STATUS) == 0)) {
        CONSOLE_PrintStatus();

        return;
    }

    if (count >= 2) {
        if (strcmp(word[1], CONSOLE_INITIALIZE) == 0) {
            word[1] = CONSOLE_INIT;
        }

        command = CONSOLE_FindCommand(word[0], word[1]);
    }

    if (command == NULL) {
        printf("FAIL: unknown command, type %s\n", CONSOLE_HELP);

        return;
    }

    if ((count - 2) != command->count) {
        printf("FAIL: usage %s %s %s\n", command->component, command->action, command->arguments);

        return;
    }

    component = CONSOLE_FindComponent(command->component);
    initialize = (strcmp(command->action, CONSOLE_INIT) == 0);

    if ((component < 0) || (!initialize && (consoleStatus[component] != CONSOLE_OK))) {
        printf("FAIL: run %s %s first\n", command->component, CONSOLE_INIT);

        return;
    }

    status = command->handler(&word[2]);

    if (initialize) {
        consoleStatus[component] = status ? CONSOLE_OK : CONSOLE_FAIL;
    }

    printf("%s\n", consoleStatusText[status ? CONSOLE_OK : CONSOLE_FAIL]);
}

static void CONSOLE_Receive(const char character) {
    bool repeated = ((character == '\n') && (lastCharacter == '\r'));

    lastCharacter = character;

    if (repeated) {
        return;
    }

    if ((character == '\r') || (character == '\n')) {
        printf("\n");

        lineBuffer[lineLength] = '\0';
        lineLength = 0;

        if (lineOverflow) {
            pyroArmed = false;
            printf("FAIL: line longer than %u characters\n", CONSOLE_BUFFER - 1);
        }
        else {
            CONSOLE_Execute(lineBuffer);
        }

        lineOverflow = false;

        printf(CONSOLE_PROMPT);
        stdio_flush();

        return;
    }

    if ((character == '\b') || (character == CONSOLE_DELETE)) {
        if (lineLength > 0) {
            lineLength--;
            printf("\b \b");
        }

        return;
    }

    if (((character >= ' ') && (character <= '~')) || (character == '\t')) {
        if (lineLength < (CONSOLE_BUFFER - 1)) {
            lineBuffer[lineLength] = character;
            lineLength++;
            putchar(character);
        }
        else {
            lineOverflow = true;
        }
    }
}

void CONSOLE_Update(void) {
    bool connected = stdio_usb_connected();
    int input;

    if (connected && !usbConnected) {
        lineLength = 0;
        lineOverflow = false;
        lastCharacter = 0;
        pyroArmed = false;
        connectTime = time_us_64();
        bannerPending = true;
    }

    usbConnected = connected;

    if (bannerPending && ((time_us_64() - connectTime) >= ((uint64_t)CONSOLE_CONNECT_MS * 1000))) {
        bannerPending = false;

        printf("\nKestrel console, type %s\n", CONSOLE_HELP);
        printf(CONSOLE_PROMPT);
        stdio_flush();
    }

    input = getchar_timeout_us(0);

    while (input != PICO_ERROR_TIMEOUT) {
        CONSOLE_Receive((char)input);
        input = getchar_timeout_us(0);
    }
}