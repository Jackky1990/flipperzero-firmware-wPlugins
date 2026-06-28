#include "astra_tests.h"
#include "astra_runtime_loopback.h"
#include "astra_runtime_codec.h"

bool astra_test_runtime_loopback(void) {
    AstraRuntimeTransport transport;
    AstraRuntimeMessage message;
    AstraRuntimeMessage decoded;

    uint8_t encoded[64];
    uint8_t received[64];
    size_t encoded_size = 0;
    size_t received_size = 0;

    const uint8_t payload[] = {0x10, 0x20, 0x30};

    if(astra_runtime_loopback_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_transport_init(&transport).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_loopback_init(&transport).status != AstraStatusOk) {
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

    if(astra_runtime_message_encode(&message, encoded, sizeof(encoded), &encoded_size).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_transport_send(&transport, encoded, encoded_size).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_transport_receive(&transport, received, sizeof(received), &received_size).status != AstraStatusOk) {
        return false;
    }

    if(received_size != encoded_size) {
        return false;
    }

    if(astra_runtime_message_decode(&decoded, received, received_size).status != AstraStatusOk) {
        return false;
    }

    if(decoded.type != message.type || decoded.source != message.source || decoded.destination != message.destination) {
        return false;
    }

    if(decoded.payload_size != sizeof(payload) || !decoded.payload) {
        return false;
    }

    const uint8_t* decoded_payload = (const uint8_t*)decoded.payload;

    return decoded_payload[0] == 0x10 &&
           decoded_payload[1] == 0x20 &&
           decoded_payload[2] == 0x30;
}
