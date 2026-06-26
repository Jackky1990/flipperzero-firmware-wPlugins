#include "astra_runtime_loop.h"
#include <stdbool.h>

static bool astra_runtime_loop_initialized = false;

AstraResult astra_runtime_loop_init(void) {
    astra_runtime_loop_initialized = true;
    return astra_result_ok();
}

AstraResult astra_runtime_loop_step(void) {
    if(!astra_runtime_loop_initialized) {
        return astra_result_error(AstraStatusInvalidArgument, "runtime loop is not initialized");
    }

    return astra_result_ok();
}

AstraResult astra_runtime_loop_shutdown(void) {
    astra_runtime_loop_initialized = false;
    return astra_result_ok();
}
