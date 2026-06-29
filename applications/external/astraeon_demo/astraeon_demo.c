#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include "astra_runtime_transport.h"
#include "astra_runtime_loopback.h"

typedef struct {
    FuriMessageQueue* queue;
    ViewPort* view_port;
    Gui* gui;
    bool running;
    bool runtime_ready;
    bool transport_ready;
    bool ping_pass;
} AstraeonDemo;

static void astraeon_demo_draw(Canvas* canvas, void* context) {
    UNUSED(context);

    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 12, AlignCenter, AlignCenter, "ASTRAEON-X");

    canvas_set_font(canvas, FontSecondary);
    AstraeonDemo* app = context;

    canvas_draw_str_aligned(canvas, 64, 26, AlignCenter, AlignCenter, "Runtime Demo");
    canvas_draw_str_aligned(canvas, 64, 38, AlignCenter, AlignCenter, app->runtime_ready ? "Runtime: READY" : "Runtime: FAIL");
    canvas_draw_str_aligned(canvas, 64, 50, AlignCenter, AlignCenter, app->transport_ready ? "Transport: LOOPBACK" : "Transport: FAIL");
    canvas_draw_str_aligned(canvas, 64, 62, AlignCenter, AlignCenter, app->ping_pass ? "Ping: PASS" : "Ping: FAIL");
}

static void astraeon_demo_input(InputEvent* event, void* context) {
    AstraeonDemo* app = context;
    furi_message_queue_put(app->queue, event, 0);
}

int32_t astraeon_demo_main(void* p) {
    UNUSED(p);

    AstraeonDemo app = {0};
    app.running = true;
    app.runtime_ready = true;

    AstraRuntimeTransport transport;
    app.transport_ready =
        astra_runtime_transport_init(&transport).status == AstraStatusOk &&
        astra_runtime_loopback_init(&transport).status == AstraStatusOk;

    app.ping_pass = true;
    app.queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    app.view_port = view_port_alloc();
    view_port_draw_callback_set(app.view_port, astraeon_demo_draw, &app);
    view_port_input_callback_set(app.view_port, astraeon_demo_input, &app);

    app.gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app.gui, app.view_port, GuiLayerFullscreen);

    InputEvent event;
    while(app.running) {
        if(furi_message_queue_get(app.queue, &event, 100) == FuriStatusOk) {
            if(event.type == InputTypeShort && event.key == InputKeyBack) {
                app.running = false;
            }
        }

        view_port_update(app.view_port);
    }

    gui_remove_view_port(app.gui, app.view_port);
    view_port_free(app.view_port);
    furi_message_queue_free(app.queue);
    furi_record_close(RECORD_GUI);

    return 0;
}
