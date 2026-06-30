#include "runtime_controller.h"

#include "astra_runtime_capabilities.h"
#include "astra_runtime_handshake.h"
#include "astra_runtime_heartbeat.h"
#include "astra_runtime_loopback.h"
#include "astra_runtime_ping.h"
#include "astra_runtime_transport.h"

#include <string.h>

static void astraeon_demo_runtime_capture_transport(
    AstraeonRuntimeContext* runtime,
    const AstraRuntimeTransport* transport) {
    const AstraRuntimeTransportCapabilities* transport_capabilities =
        astra_runtime_transport_capabilities(transport);

    if(!transport_capabilities) {
        return;
    }

    runtime->supports_stream = transport_capabilities->supports_stream;
    runtime->supports_packets = transport_capabilities->supports_packets;
    runtime->reliable = transport_capabilities->reliable;
    runtime->max_payload_size = transport_capabilities->max_payload_size;
}

static bool astraeon_demo_runtime_check_capabilities(AstraeonRuntimeContext* runtime) {
    AstraRuntimeCapabilities local_capabilities;
    AstraRuntimeCapabilities remote_capabilities;
    AstraRuntimeCapabilities agreed_capabilities;

    if(astra_runtime_capabilities_init(&local_capabilities).status != AstraStatusOk ||
       astra_runtime_capabilities_init(&remote_capabilities).status != AstraStatusOk) {
        return false;
    }

    local_capabilities.supports_stream = runtime->supports_stream;
    local_capabilities.supports_packets = runtime->supports_packets;
    local_capabilities.reliable = runtime->reliable;
    local_capabilities.max_payload_size = runtime->max_payload_size;

    remote_capabilities = local_capabilities;

    return astra_runtime_handshake_negotiate(
               &local_capabilities,
               &remote_capabilities,
               &agreed_capabilities)
               .status == AstraStatusOk;
}

static bool astraeon_demo_runtime_check_handshake(AstraeonRuntimeContext* runtime) {
    AstraRuntimeHandshake handshake;

    if(astra_runtime_handshake_init(&handshake).status != AstraStatusOk) {
        return false;
    }

    handshake.max_payload_size = runtime->max_payload_size;
    runtime->protocol_version = handshake.protocol_version;

    return astra_runtime_handshake_validate(&handshake).status == AstraStatusOk;
}

static bool astraeon_demo_runtime_check_heartbeat(void) {
    AstraRuntimeHeartbeat heartbeat;

    return astra_runtime_heartbeat_init(&heartbeat, 1000, 3000).status == AstraStatusOk &&
           !astra_runtime_heartbeat_has_timed_out(&heartbeat, 1000) &&
           astra_runtime_heartbeat_should_send(&heartbeat, 1000);
}

void astraeon_demo_runtime_controller_start(AstraeonRuntimeContext* runtime) {
    if(!runtime) {
        return;
    }

    uint32_t diagnostic_runs = runtime->diagnostic_runs + 1;
    memset(runtime, 0, sizeof(*runtime));
    runtime->diagnostic_runs = diagnostic_runs;

    AstraRuntimeTransport transport;

    runtime->ready = true;
    runtime->transport_ready =
        astra_runtime_transport_init(&transport).status == AstraStatusOk &&
        astra_runtime_loopback_init(&transport).status == AstraStatusOk;

    if(runtime->transport_ready) {
        astraeon_demo_runtime_capture_transport(runtime, &transport);
    }

    runtime->capabilities_ok = astraeon_demo_runtime_check_capabilities(runtime);
    runtime->handshake_ok =
        runtime->capabilities_ok && astraeon_demo_runtime_check_handshake(runtime);
    runtime->heartbeat_ok = astraeon_demo_runtime_check_heartbeat();

    runtime->ping_ok =
        runtime->transport_ready &&
        astra_runtime_ping_send(&transport).status == AstraStatusOk &&
        astra_runtime_ping_receive(&transport).status == AstraStatusOk;
}
