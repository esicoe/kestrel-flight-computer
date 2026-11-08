#include "rak3172.h"

static const char *rak3172Config[] = {
    OPCODE_LORA_LOW_POWER_MODE VALUE_LORA_LPM_OFF,
    OPCODE_LORA_P2P_PARAMETERS VALUE_LORA_FREQUENCY ":" VALUE_LORA_SF_7 ":" VALUE_LORA_BW_500 ":" VALUE_LORA_CR_4_5 ":" VALUE_LORA_PREAMBLE ":" VALUE_LORA_TX_POWER,
    OPCODE_LORA_P2P_RECEIVE VALUE_LORA_PRECV_RX_TX,
};

static const char hexDigits[] = "0123456789ABCDEF";

static char lineBuffer[VALUE_LORA_LINE_SIZE];
static uint16_t lineLength = 0;
static bool lineDropped = false;
static char responseBuffer[VALUE_LORA_LINE_SIZE];
static char commandBuffer[sizeof(OPCODE_LORA_P2P_SEND) + (2 * VALUE_LORA_PAYLOAD_SIZE)];
static rak3172_packet_t rxPacket;
static bool rxReady = false;
static bool transmitting = false;
static uint64_t txStart = 0;

static bool RAK3172_GetLine(void) {
    uint8_t data;

    if (UART1_IsOverflowed()) {
        UART1_Flush();
        lineDropped = true;
    }

    while (UART1_Read(&data)) {
        if (data == '\n') {
            lineBuffer[lineLength] = '\0';
            lineLength = 0;

            if (!lineDropped) {
                return true;
            }

            lineDropped = false;
        }

        if ((data != '\n') && (data != '\r') && (lineLength < (VALUE_LORA_LINE_SIZE - 1))) {
            lineBuffer[lineLength] = (char)data;
            lineLength++;
        }
    }

    return false;
}

static int8_t RAK3172_HexValue(const char c) {
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

static void RAK3172_ParsePacket(const char *text) {
    rak3172_packet_t packet = {0};
    char *cursor;
    int8_t high;
    int8_t low;

    packet.time_us = time_us_64();

    packet.rssi_dBm = (int16_t)strtol(text, &cursor, 10);
    if (*cursor != ':') {
        return;
    }

    packet.snr_dB = (int8_t)strtol(cursor + 1, &cursor, 10);
    if (*cursor != ':') {
        return;
    }

    cursor++;

    while (*cursor != '\0') {
        high = RAK3172_HexValue(cursor[0]);
        low = RAK3172_HexValue(cursor[1]);

        if ((high < 0) || (low < 0) || (packet.length >= VALUE_LORA_PAYLOAD_SIZE)) {
            return;
        }

        packet.payload[packet.length] = (uint8_t)((high << 4) | low);
        packet.length++;
        cursor += 2;
    }

    rxPacket = packet;
    rxReady = true;
}

static void RAK3172_ParseEvent(void) {
    if (strcmp(lineBuffer, OPCODE_LORA_EVENT_TX_DONE) == 0) {
        transmitting = false;
    }

    if (strncmp(lineBuffer, OPCODE_LORA_EVENT_RX ":", sizeof(OPCODE_LORA_EVENT_RX ":") - 1) == 0) {
        RAK3172_ParsePacket(lineBuffer + sizeof(OPCODE_LORA_EVENT_RX ":") - 1);
    }
}

static void RAK3172_ProcessEvents(void) {
    while (RAK3172_GetLine()) {
        RAK3172_ParseEvent();
    }
}

static bool RAK3172_WaitResponse(const uint32_t timeout) {
    uint64_t deadline = time_us_64() + ((uint64_t)timeout * 1000);

    while (time_us_64() < deadline) {
        if (RAK3172_GetLine()) {
            if (strcmp(lineBuffer, VALUE_LORA_OK) == 0) {
                return true;
            }

            if (strncmp(lineBuffer, VALUE_LORA_ERROR, sizeof(VALUE_LORA_ERROR) - 1) == 0) {
                return false;
            }

            if (lineBuffer[0] != '\0') {
                strcpy(responseBuffer, lineBuffer);
            }

            RAK3172_ParseEvent();
        }
    }

    return false;
}

static bool RAK3172_SendCommand(const char *command, const uint32_t timeout) {
    RAK3172_ProcessEvents();

    UART1_Write((const uint8_t *)command, strlen(command));
    UART1_Write((const uint8_t *)VALUE_LORA_END, sizeof(VALUE_LORA_END) - 1);

    return RAK3172_WaitResponse(timeout);
}

static bool RAK3172_Synchronize(void) {
    for (uint8_t i = 0; i < VALUE_LORA_SYNC_ATTEMPTS; i++) {
        if (RAK3172_SendCommand(OPCODE_LORA_AT, VALUE_LORA_SYNC_MS)) {
            return true;
        }
    }

    return false;
}

void RAK3172_Initialize(void) {
    if (!RAK3172_Configure()) {
        LED_DisplayError(LORA_FAIL);
    }
}

bool RAK3172_Configure(void) {
    bool status = true;

    gpio_init(PIN_LORA_RST);
    gpio_disable_pulls(PIN_LORA_RST);
    gpio_put(PIN_LORA_RST, PIN_LOW);
    gpio_set_dir(PIN_LORA_RST, GPIO_OUT);

    sleep_ms(VALUE_LORA_RESET_MS);

    gpio_set_dir(PIN_LORA_RST, GPIO_IN);

    sleep_ms(VALUE_LORA_BOOT_MS);

    UART1_Flush();
    lineLength = 0;
    lineDropped = false;
    rxReady = false;
    transmitting = false;

    status &= RAK3172_Synchronize();

    responseBuffer[0] = '\0';
    status &= RAK3172_SendCommand(OPCODE_LORA_NETWORK_MODE VALUE_LORA_QUERY, VALUE_LORA_RESPONSE_MS);

    if (strcmp(responseBuffer, OPCODE_LORA_NETWORK_MODE VALUE_LORA_NWM_P2P) != 0) {
        (void)RAK3172_SendCommand(OPCODE_LORA_NETWORK_MODE VALUE_LORA_NWM_P2P, VALUE_LORA_RESPONSE_MS);

        sleep_ms(VALUE_LORA_BOOT_MS);

        status &= RAK3172_Synchronize();
    }

    for (uint8_t i = 0; i < (sizeof(rak3172Config) / sizeof(rak3172Config[0])); i++) {
        status &= RAK3172_SendCommand(rak3172Config[i], VALUE_LORA_RESPONSE_MS);
    }

    return status;
}

bool RAK3172_Send(const uint8_t *data, const uint8_t length) {
    uint16_t position = sizeof(OPCODE_LORA_P2P_SEND) - 1;

    if ((length == 0) || (length > VALUE_LORA_PAYLOAD_SIZE)) {
        return false;
    }

    memcpy(commandBuffer, OPCODE_LORA_P2P_SEND, position);

    for (uint8_t i = 0; i < length; i++) {
        commandBuffer[position] = hexDigits[data[i] >> 4];
        commandBuffer[position + 1] = hexDigits[data[i] & 0x0F];
        position += 2;
    }

    commandBuffer[position] = '\0';

    if (!RAK3172_SendCommand(commandBuffer, VALUE_LORA_RESPONSE_MS)) {
        return false;
    }

    transmitting = true;
    txStart = time_us_64();

    return true;
}

bool RAK3172_IsTransmitting(void) {
    RAK3172_ProcessEvents();

    if (transmitting && ((time_us_64() - txStart) > ((uint64_t)VALUE_LORA_TX_TIMEOUT_MS * 1000))) {
        transmitting = false;
    }

    return transmitting;
}

bool RAK3172_Receive(rak3172_packet_t *packet) {
    RAK3172_ProcessEvents();

    if (!rxReady) {
        return false;
    }

    *packet = rxPacket;
    rxReady = false;

    return true;
}