#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include "app_state.h"
#include "view.h"
#include "input.h"
#include "astra_runtime_transport.h"
#include "astra_runtime_loopback.h"
#include "astra_runtime_ping.h"


int32_t astraeon_demo_main(void* p) {
    UNUSED(p);

    AstraeonDemo app = {0};
    app.running = true;
    app.runtime_ready = true;

    AstraRuntimeTransport transport;
    app.transport_ready =
        astra_runtime_transport_init(&transport).status == AstraStatusOk &&
        astra_runtime_loopback_init(&transport).status == AstraStatusOk;

    app.ping_pass =
        app.transport_ready &&
        astra_runtime_ping_send(&transport).status == AstraStatusOk &&
        astra_runtime_ping_receive(&transport).status == AstraStatusOk;
    app.queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    app.view_port = view_port_alloc();
    view_port_draw_callback_set(app.view_port, astraeon_demo_draw, &app);
    view_port_input_callback_set(app.view_port, astraeon_demo_input, &app);

    app.gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app.gui, app.view_port, GuiLayerFullscreen);

    InputEvent event;
    while(app.running) {
        if(furi_message_queue_get(app.queue, &event, 100) == FuriStatusOk) {
            astraeon_demo_handle_input(&event, &app);
        }

        view_port_update(app.view_port);
    }

    gui_remove_view_port(app.gui, app.view_port);
    view_port_free(app.view_port);
    furi_message_queue_free(app.queue);
    furi_record_close(RECORD_GUI);

    return 0;
}
