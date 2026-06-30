#include "runtime_bootstrap.h"

#include "astra_runtime_transport.h"
#include "astra_runtime_loopback.h"
#include "astra_runtime_ping.h"
#include "astra_runtime_capabilities.h"
#include "astra_runtime_handshake.h"
#include "astra_runtime_heartbeat.h"

void astraeon_demo_runtime_bootstrap(AstraeonDemo* app) {
    if(!app) {
        return;
    }

    AstraeonRuntimeContext* runtime = &app->app.runtime;

    AstraRuntimeTransport transport;
    runtime->ready = true;
    runtime->transport_ready =
        astra_runtime_transport_init(&transport).status == AstraStatusOk &&
        astra_runtime_loopback_init(&transport).status == AstraStatusOk;

    if(runtime->transport_ready) {
        const AstraRuntimeTransportCapabilities* transport_capabilities =
            astra_runtime_transport_capabilities(&transport);
        if(transport_capabilities) {
            runtime->supports_stream = transport_capabilities->supports_stream;
            runtime->supports_packets = transport_capabilities->supports_packets;
            runtime->reliable = transport_capabilities->reliable;
            runtime->max_payload_size = transport_capabilities->max_payload_size;
        }
    }

    AstraRuntimeCapabilities local_capabilities;
    AstraRuntimeCapabilities remote_capabilities;
    AstraRuntimeCapabilities agreed_capabilities;
    runtime->capabilities_ok =
        astra_runtime_capabilities_init(&local_capabilities).status == AstraStatusOk &&
        astra_runtime_capabilities_init(&remote_capabilities).status == AstraStatusOk;

    if(runtime->capabilities_ok) {
        local_capabilities.supports_stream = runtime->supports_stream;
        local_capabilities.supports_packets = runtime->supports_packets;
        local_capabilities.reliable = runtime->reliable;
        local_capabilities.max_payload_size = runtime->max_payload_size;

        remote_capabilities = local_capabilities;

        runtime->capabilities_ok =
            astra_runtime_handshake_negotiate(
                &local_capabilities,
                &remote_capabilities,
                &agreed_capabilities)
                .status == AstraStatusOk;
    }

    AstraRuntimeHandshake handshake;
    runtime->handshake_ok =
        runtime->capabilities_ok &&
        astra_runtime_handshake_init(&handshake).status == AstraStatusOk;

    if(runtime->handshake_ok) {
        handshake.max_payload_size = runtime->max_payload_size;
        runtime->handshake_ok =
            astra_runtime_handshake_validate(&handshake).status == AstraStatusOk;
        runtime->protocol_version = handshake.protocol_version;
    }

    AstraRuntimeHeartbeat heartbeat;
    runtime->heartbeat_ok =
        astra_runtime_heartbeat_init(&heartbeat, 1000, 3000).status == AstraStatusOk &&
        !astra_runtime_heartbeat_has_timed_out(&heartbeat, 1000) &&
        astra_runtime_heartbeat_should_send(&heartbeat, 1000);

    runtime->ping_ok =
        runtime->transport_ready &&
        astra_runtime_ping_send(&transport).status == AstraStatusOk &&
        astra_runtime_ping_receive(&transport).status == AstraStatusOk;
}
