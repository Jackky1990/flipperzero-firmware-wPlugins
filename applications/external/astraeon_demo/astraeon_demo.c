#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include "app_state.h"
#include "view.h"
#include "input.h"
#include "runtime_bootstrap.h"
#include "application/application.h"


int32_t astraeon_demo_main(void* p) {
    UNUSED(p);

    AstraeonDemo app = {0};
    astraeon_application_bind(&app);
    app.running = true;
    astraeon_demo_runtime_bootstrap(&app);
    app.app.platform.queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    app.view_port = view_port_alloc();
    view_port_draw_callback_set(app.view_port, astraeon_demo_draw, &app);
    view_port_input_callback_set(app.view_port, astraeon_demo_input, &app);

    app.gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app.gui, app.view_port, GuiLayerFullscreen);

    InputEvent event;
    while(astraeon_application_is_running()) {
        if(furi_message_queue_get(app.app.platform.queue, &event, 100) == FuriStatusOk) {
            astraeon_demo_handle_input(&event, &app);
        }

        view_port_update(app.view_port);
    }

    gui_remove_view_port(app.gui, app.view_port);
    view_port_free(app.view_port);
    furi_message_queue_free(app.app.platform.queue);
    furi_record_close(RECORD_GUI);

    return 0;
}
