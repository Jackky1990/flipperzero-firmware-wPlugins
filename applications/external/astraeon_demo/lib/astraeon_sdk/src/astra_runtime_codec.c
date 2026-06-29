#include "astra_runtime_codec.h"

#include <string.h>

static void astra_write_u16(uint8_t* buffer, uint16_t value) {
    buffer[0] = (uint8_t)((value >> 8) & 0xFFu);
    buffer[1] = (uint8_t)(value & 0xFFu);
}

static void astra_write_u32(uint8_t* buffer, uint32_t value) {
    buffer[0] = (uint8_t)((value >> 24) & 0xFFu);
    buffer[1] = (uint8_t)((value >> 16) & 0xFFu);
    buffer[2] = (uint8_t)((value >> 8) & 0xFFu);
    buffer[3] = (uint8_t)(value & 0xFFu);
}

static uint16_t astra_read_u16(const uint8_t* buffer) {
    return (uint16_t)(((uint16_t)buffer[0] << 8) | buffer[1]);
}

static uint32_t astra_read_u32(const uint8_t* buffer) {
    return ((uint32_t)buffer[0] << 24) |
           ((uint32_t)buffer[1] << 16) |
           ((uint32_t)buffer[2] << 8) |
           (uint32_t)buffer[3];
}

AstraResult astra_runtime_message_encode(
    const AstraRuntimeMessage* message,
    uint8_t* buffer,
    size_t buffer_size,
    size_t* encoded_size) {
    AstraResult validation = astra_runtime_message_validate(message);
    if(validation.status != AstraStatusOk) {
        return validation;
    }

    if(!buffer || !encoded_size) {
        return astra_result_error(AstraStatusInvalidArgument, "output is null");
    }

    size_t required = ASTRA_RUNTIME_CODEC_HEADER_SIZE + message->payload_size;
    if(buffer_size < required) {
        return astra_result_error(AstraStatusBusy, "buffer too small");
    }

    astra_write_u16(buffer + 0, ASTRA_RUNTIME_CODEC_MAGIC);
    buffer[2] = ASTRA_RUNTIME_CODEC_VERSION;
    buffer[3] = (uint8_t)message->type;
    astra_write_u32(buffer + 4, message->source);
    astra_write_u32(buffer + 8, message->destination);
    astra_write_u16(buffer + 12, message->payload_size);
    astra_write_u16(buffer + 14, 0);

    if(message->payload_size > 0) {
        memcpy(buffer + ASTRA_RUNTIME_CODEC_HEADER_SIZE, message->payload, message->payload_size);
    }

    *encoded_size = required;
    return astra_result_ok();
}

AstraResult astra_runtime_message_decode(
    AstraRuntimeMessage* message,
    const uint8_t* buffer,
    size_t buffer_size) {
    if(!message || !buffer) {
        return astra_result_error(AstraStatusInvalidArgument, "input is null");
    }

    if(buffer_size < ASTRA_RUNTIME_CODEC_HEADER_SIZE) {
        return astra_result_error(AstraStatusInvalidArgument, "buffer too small");
    }

    if(astra_read_u16(buffer + 0) != ASTRA_RUNTIME_CODEC_MAGIC) {
        return astra_result_error(AstraStatusInvalidArgument, "bad magic");
    }

    if(buffer[2] != ASTRA_RUNTIME_CODEC_VERSION) {
        return astra_result_error(AstraStatusInvalidArgument, "bad version");
    }

    uint16_t payload_size = astra_read_u16(buffer + 12);
    if(buffer_size < ASTRA_RUNTIME_CODEC_HEADER_SIZE + payload_size) {
        return astra_result_error(AstraStatusInvalidArgument, "payload truncated");
    }

    message->type = (AstraRuntimeMessageType)buffer[3];
    message->source = astra_read_u32(buffer + 4);
    message->destination = astra_read_u32(buffer + 8);
    message->payload_size = payload_size;
    message->payload = payload_size > 0 ? buffer + ASTRA_RUNTIME_CODEC_HEADER_SIZE : 0;

    return astra_runtime_message_validate(message);
}
