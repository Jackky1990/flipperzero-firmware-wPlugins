#include "input.h"
#include "app_state.h"
#include "application/application.h"

void astraeon_demo_input(InputEvent* event, void* context) {
    AstraeonDemo* app = context;
    furi_message_queue_put(app->app.platform.queue, event, 0);
}

void astraeon_demo_handle_input(InputEvent* event, void* context) {
    AstraeonDemo* app = context;

    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        astraeon_application_stop();
    } else if(event->type == InputTypeShort && event->key == InputKeyDown) {
        if(app->app.ui.menu_index < 4) {
            app->app.ui.menu_index++;
            astraeon_application_request_redraw();
        }
    } else if(event->type == InputTypeShort && event->key == InputKeyUp) {
        if(app->app.ui.menu_index > 0) {
            app->app.ui.menu_index--;
            astraeon_application_request_redraw();
        }
    }
}
