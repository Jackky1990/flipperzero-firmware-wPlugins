#include "runtime_bootstrap.h"

#include "astra_runtime_transport.h"
#include "astra_runtime_loopback.h"
#include "astra_runtime_ping.h"

void astraeon_demo_runtime_bootstrap(AstraeonDemo* app) {
    if(!app) {
        return;
    }

    app->runtime_ready = true;

    AstraRuntimeTransport transport;
    app->transport_ready =
        astra_runtime_transport_init(&transport).status == AstraStatusOk &&
        astra_runtime_loopback_init(&transport).status == AstraStatusOk;

    app->ping_pass =
        app->transport_ready &&
        astra_runtime_ping_send(&transport).status == AstraStatusOk &&
        astra_runtime_ping_receive(&transport).status == AstraStatusOk;
}
