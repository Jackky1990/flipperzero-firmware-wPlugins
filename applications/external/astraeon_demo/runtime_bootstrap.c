#include "runtime_bootstrap.h"

#include "runtime_controller.h"

void astraeon_demo_runtime_bootstrap(AstraeonDemo* app) {
    if(!app) {
        return;
    }

    astraeon_demo_runtime_controller_start(&app->app.runtime, app->app.platform.storage);
}
