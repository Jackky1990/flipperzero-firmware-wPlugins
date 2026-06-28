#include "astra_tests.h"
#include "astra_runtime_codec.h"

bool astra_test_runtime_codec(void) {
    AstraRuntimeMessage message;
    AstraRuntimeMessage decoded;
    uint8_t buffer[64];
    size_t encoded_size = 0;
    const uint8_t payload[] = {0xAA, 0xBB, 0xCC};

    if(astra_runtime_message_encode(0, buffer, sizeof(buffer), &encoded_size).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_message_decode(0, buffer, sizeof(buffer)).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_message_init(&message).status != AstraStatusOk) {
        return false;
    }

    message.type = AstraRuntimeMessageCommand;
    message.source = 1;
    message.destination = 2;
    message.payload_size = sizeof(payload);
    message.payload = payload;

    if(astra_runtime_message_encode(&message, 0, sizeof(buffer), &encoded_size).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_message_encode(&message, buffer, 4, &encoded_size).status != AstraStatusBusy) {
        return false;
    }

    if(astra_runtime_message_encode(&message, buffer, sizeof(buffer), &encoded_size).status != AstraStatusOk) {
        return false;
    }

    if(encoded_size != ASTRA_RUNTIME_CODEC_HEADER_SIZE + sizeof(payload)) {
        return false;
    }

    if(astra_runtime_message_decode(&decoded, buffer, encoded_size).status != AstraStatusOk) {
        return false;
    }

    if(decoded.type != message.type || decoded.source != message.source || decoded.destination != message.destination) {
        return false;
    }

    if(decoded.payload_size != sizeof(payload)) {
        return false;
    }

    if(!decoded.payload) {
        return false;
    }

    const uint8_t* decoded_payload = (const uint8_t*)decoded.payload;
    if(decoded_payload[0] != 0xAA ||
       decoded_payload[1] != 0xBB ||
       decoded_payload[2] != 0xCC) {
        return false;
    }

    uint8_t bad_magic[64];
    for(size_t i = 0; i < encoded_size; i++) {
        bad_magic[i] = buffer[i];
    }
    bad_magic[0] = 0x00;

    if(astra_runtime_message_decode(&decoded, bad_magic, encoded_size).status != AstraStatusInvalidArgument) {
        return false;
    }

    uint8_t bad_version[64];
    for(size_t i = 0; i < encoded_size; i++) {
        bad_version[i] = buffer[i];
    }
    bad_version[2] = 0xFF;

    if(astra_runtime_message_decode(&decoded, bad_version, encoded_size).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_message_decode(&decoded, buffer, ASTRA_RUNTIME_CODEC_HEADER_SIZE - 1).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_message_decode(&decoded, buffer, encoded_size - 1).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_message_init(&message).status != AstraStatusOk) {
        return false;
    }

    message.type = AstraRuntimeMessageHeartbeat;
    message.source = 1;
    message.destination = 2;
    message.payload_size = 0;
    message.payload = 0;

    if(astra_runtime_message_encode(&message, buffer, sizeof(buffer), &encoded_size).status != AstraStatusOk) {
        return false;
    }

    if(encoded_size != ASTRA_RUNTIME_CODEC_HEADER_SIZE) {
        return false;
    }

    if(astra_runtime_message_decode(&decoded, buffer, encoded_size).status != AstraStatusOk) {
        return false;
    }

    return decoded.type == AstraRuntimeMessageHeartbeat &&
           decoded.payload_size == 0 &&
           decoded.payload == 0;
}
