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


AstraResult astra_runtime_handshake_negotiate(
    const AstraRuntimeCapabilities* local,
    const AstraRuntimeCapabilities* remote,
    AstraRuntimeCapabilities* agreed) {
    if(!local || !remote || !agreed) {
        return astra_result_error(AstraStatusInvalidArgument, "negotiation input is null");
    }

    AstraResult local_result = astra_runtime_capabilities_validate(local);
    if(local_result.status != AstraStatusOk) {
        return local_result;
    }

    AstraResult remote_result = astra_runtime_capabilities_validate(remote);
    if(remote_result.status != AstraStatusOk) {
        return remote_result;
    }

    AstraResult init_result = astra_runtime_capabilities_init(agreed);
    if(init_result.status != AstraStatusOk) {
        return init_result;
    }

    agreed->supports_stream = local->supports_stream && remote->supports_stream;
    agreed->supports_packets = local->supports_packets && remote->supports_packets;
    agreed->reliable = local->reliable && remote->reliable;
    agreed->max_payload_size =
        local->max_payload_size < remote->max_payload_size
            ? local->max_payload_size
            : remote->max_payload_size;
    agreed->feature_flags = local->feature_flags & remote->feature_flags;

    return astra_runtime_capabilities_validate(agreed);
}
