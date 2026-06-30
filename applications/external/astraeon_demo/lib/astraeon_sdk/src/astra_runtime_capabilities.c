#include "astra_runtime_capabilities.h"
#include "astra_runtime_handshake.h"

AstraResult astra_runtime_capabilities_init(AstraRuntimeCapabilities* capabilities) {
    if(!capabilities) {
        return astra_result_error(AstraStatusInvalidArgument, "capabilities is null");
    }

    capabilities->protocol_version = ASTRA_RUNTIME_PROTOCOL_VERSION;
    capabilities->supports_stream = false;
    capabilities->supports_packets = false;
    capabilities->reliable = false;
    capabilities->max_payload_size = 0;
    capabilities->feature_flags = ASTRA_RUNTIME_FEATURE_NONE;

    return astra_result_ok();
}

AstraResult astra_runtime_capabilities_validate(const AstraRuntimeCapabilities* capabilities) {
    if(!capabilities) {
        return astra_result_error(AstraStatusInvalidArgument, "capabilities is null");
    }

    if(capabilities->protocol_version != ASTRA_RUNTIME_PROTOCOL_VERSION) {
        return astra_result_error(AstraStatusInvalidArgument, "protocol version mismatch");
    }

    if(capabilities->max_payload_size == 0) {
        return astra_result_error(AstraStatusInvalidArgument, "max payload size is invalid");
    }

    return astra_result_ok();
}

bool astra_runtime_capabilities_supports_stream(const AstraRuntimeCapabilities* capabilities) {
    return capabilities && capabilities->supports_stream;
}

bool astra_runtime_capabilities_supports_packets(const AstraRuntimeCapabilities* capabilities) {
    return capabilities && capabilities->supports_packets;
}
