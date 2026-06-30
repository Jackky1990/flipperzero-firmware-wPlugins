#include "input.h"
#include "app_state.h"
#include "application/application.h"
#include "screen/screen_manager.h"

void astraeon_demo_input(InputEvent* event, void* context) {
    AstraeonDemo* app = context;
    furi_message_queue_put(app->app.platform.queue, event, 0);
}

void astraeon_demo_handle_input(InputEvent* event, void* context) {
    AstraeonDemo* app = context;

    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        astraeon_application_stop();
    } else if(event->type == InputTypeShort && event->key == InputKeyDown) {
        app->app.ui.current_screen =
            astraeon_screen_next((AstraeonScreen)app->app.ui.current_screen);
        astraeon_application_request_redraw();
    } else if(event->type == InputTypeShort && event->key == InputKeyUp) {
        app->app.ui.current_screen =
            astraeon_screen_previous((AstraeonScreen)app->app.ui.current_screen);
        astraeon_application_request_redraw();
    } else if(astraeon_screen_handle_input(
                  (AstraeonScreen)app->app.ui.current_screen,
                  event,
                  app)) {
        astraeon_application_request_redraw();
    }
}
