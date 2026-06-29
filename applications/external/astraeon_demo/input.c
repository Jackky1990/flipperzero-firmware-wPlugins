#include "input.h"
#include "app_state.h"

void astraeon_demo_input(InputEvent* event, void* context) {
    AstraeonDemo* app = context;
    furi_message_queue_put(app->queue, event, 0);
}

void astraeon_demo_handle_input(InputEvent* event, void* context) {
    AstraeonDemo* app = context;

    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        app->running = false;
    } else if(event->type == InputTypeShort && event->key == InputKeyDown) {
        if(app->menu_index < 4) {
            app->menu_index++;
        }
    } else if(event->type == InputTypeShort && event->key == InputKeyUp) {
        if(app->menu_index > 0) {
            app->menu_index--;
        }
    }
}
