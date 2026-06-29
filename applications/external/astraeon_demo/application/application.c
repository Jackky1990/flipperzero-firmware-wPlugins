#include "application.h"

static AstraeonDemo* astraeon_current_app = 0;

void astraeon_application_bind(AstraeonDemo* app) {
    astraeon_current_app = app;
}

AstraeonDemo* astraeon_application_get(void) {
    return astraeon_current_app;
}

void astraeon_application_stop(void) {
    if(astraeon_current_app) {
        astraeon_current_app->running = false;
    }
}

bool astraeon_application_is_running(void) {
    return astraeon_current_app && astraeon_current_app->running;
}

void astraeon_application_request_redraw(void) {
    if(astraeon_current_app && astraeon_current_app->view_port) {
        view_port_update(astraeon_current_app->view_port);
    }
}
