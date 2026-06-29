#include "astra_runtime_loopback.h"

#include <string.h>

#define ASTRA_LOOPBACK_BUFFER_SIZE 128

static uint8_t loopback_buffer[ASTRA_LOOPBACK_BUFFER_SIZE];
static size_t loopback_size = 0;

static AstraResult astra_loopback_send(const uint8_t* data, size_t size) {
    if(!data || size == 0 || size > ASTRA_LOOPBACK_BUFFER_SIZE) {
        return astra_result_error(AstraStatusInvalidArgument, "loopback send invalid");
    }

    memcpy(loopback_buffer, data, size);
    loopback_size = size;
    return astra_result_ok();
}

static AstraResult astra_loopback_receive(uint8_t* buffer, size_t buffer_size, size_t* received_size) {
    if(!buffer || !received_size || buffer_size < loopback_size) {
        return astra_result_error(AstraStatusInvalidArgument, "loopback receive invalid");
    }

    memcpy(buffer, loopback_buffer, loopback_size);
    *received_size = loopback_size;
    return astra_result_ok();
}

AstraResult astra_runtime_loopback_init(AstraRuntimeTransport* transport) {
    if(!transport) {
        return astra_result_error(AstraStatusInvalidArgument, "transport is null");
    }

    loopback_size = 0;
    transport->send = astra_loopback_send;
    transport->receive = astra_loopback_receive;
    transport->capabilities.supports_stream = false;
    transport->capabilities.supports_packets = true;
    transport->capabilities.reliable = true;
    transport->capabilities.max_payload_size = ASTRA_LOOPBACK_BUFFER_SIZE;
    return astra_result_ok();
}
