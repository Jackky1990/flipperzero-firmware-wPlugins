#include "astra_runtime_handshake.h"

AstraResult astra_runtime_handshake_init(AstraRuntimeHandshake* handshake) {
    if(!handshake) {
        return astra_result_error(AstraStatusInvalidArgument, "handshake is null");
    }

    handshake->protocol_version = ASTRA_RUNTIME_PROTOCOL_VERSION;
    handshake->capability_flags = 0;
    handshake->runtime_id = 0;
    handshake->max_payload_size = 0;

    return astra_result_ok();
}

AstraResult astra_runtime_handshake_validate(const AstraRuntimeHandshake* handshake) {
    if(!handshake) {
        return astra_result_error(AstraStatusInvalidArgument, "handshake is null");
    }

    if(handshake->protocol_version != ASTRA_RUNTIME_PROTOCOL_VERSION) {
        return astra_result_error(AstraStatusInvalidArgument, "protocol version mismatch");
    }

    if(handshake->max_payload_size == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "max payload size is invalid");
    }

    return astra_result_ok();
}

uint16_t astra_runtime_protocol_version(void) {
    return ASTRA_RUNTIME_PROTOCOL_VERSION;
}
