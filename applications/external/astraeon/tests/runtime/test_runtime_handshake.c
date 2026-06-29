#include "astra_tests.h"
#include "astra_runtime_handshake.h"
#include "astra_runtime_capabilities.h"

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

    if(astra_runtime_handshake_validate(&handshake).status != AstraStatusInvalidArgument) {
        return false;
    }

    AstraRuntimeCapabilities local;
    AstraRuntimeCapabilities remote;
    AstraRuntimeCapabilities agreed;

    if(astra_runtime_capabilities_init(&local).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_capabilities_init(&remote).status != AstraStatusOk) {
        return false;
    }

    local.supports_stream = true;
    local.supports_packets = true;
    local.reliable = true;
    local.max_payload_size = 128;
    local.feature_flags = 0x03;

    remote.supports_stream = true;
    remote.supports_packets = false;
    remote.reliable = true;
    remote.max_payload_size = 64;
    remote.feature_flags = 0x01;

    if(astra_runtime_handshake_negotiate(0, &remote, &agreed).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_handshake_negotiate(&local, &remote, &agreed).status != AstraStatusOk) {
        return false;
    }

    return agreed.supports_stream &&
           !agreed.supports_packets &&
           agreed.reliable &&
           agreed.max_payload_size == 64 &&
           agreed.feature_flags == 0x01;
}
