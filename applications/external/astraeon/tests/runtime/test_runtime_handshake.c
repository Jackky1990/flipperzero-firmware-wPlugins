#include "astra_tests.h"
#include "astra_runtime_handshake.h"

bool astra_test_runtime_handshake(void) {
    AstraRuntimeHandshake handshake;

    if(astra_runtime_handshake_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_handshake_validate(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_protocol_version() != ASTRA_RUNTIME_PROTOCOL_VERSION) {
        return false;
    }

    if(astra_runtime_handshake_init(&handshake).status != AstraStatusOk) {
        return false;
    }

    if(handshake.protocol_version != ASTRA_RUNTIME_PROTOCOL_VERSION) {
        return false;
    }

    if(astra_runtime_handshake_validate(&handshake).status != AstraStatusInvalidArgument) {
        return false;
    }

    handshake.max_payload_size = 64;

    if(astra_runtime_handshake_validate(&handshake).status != AstraStatusOk) {
        return false;
    }

    handshake.protocol_version = ASTRA_RUNTIME_PROTOCOL_VERSION + 1;

    return astra_runtime_handshake_validate(&handshake).status == AstraStatusInvalidArgument;
}
