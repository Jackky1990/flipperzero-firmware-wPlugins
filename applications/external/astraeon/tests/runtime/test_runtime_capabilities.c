#include "astra_tests.h"
#include "astra_runtime_capabilities.h"
#include "astra_runtime_handshake.h"

bool astra_test_runtime_capabilities(void) {
    AstraRuntimeCapabilities capabilities;

    if(astra_runtime_capabilities_init(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_capabilities_validate(0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_capabilities_supports_stream(0)) {
        return false;
    }

    if(astra_runtime_capabilities_supports_packets(0)) {
        return false;
    }

    if(astra_runtime_capabilities_init(&capabilities).status != AstraStatusOk) {
        return false;
    }

    if(capabilities.protocol_version != ASTRA_RUNTIME_PROTOCOL_VERSION) {
        return false;
    }

    if(astra_runtime_capabilities_validate(&capabilities).status != AstraStatusInvalidArgument) {
        return false;
    }

    capabilities.max_payload_size = 64;
    capabilities.supports_stream = true;
    capabilities.supports_packets = true;
    capabilities.reliable = true;

    if(!astra_runtime_capabilities_supports_stream(&capabilities)) {
        return false;
    }

    if(!astra_runtime_capabilities_supports_packets(&capabilities)) {
        return false;
    }

    if(astra_runtime_capabilities_validate(&capabilities).status != AstraStatusOk) {
        return false;
    }

    capabilities.protocol_version = ASTRA_RUNTIME_PROTOCOL_VERSION + 1;

    return astra_runtime_capabilities_validate(&capabilities).status == AstraStatusInvalidArgument;
}
