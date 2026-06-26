#include "astra_runtime_loop.h"
#include "astra_event_bus.h"
#include "astra_scheduler.h"
#include <stdbool.h>

static bool astra_runtime_loop_initialized = false;

AstraResult astra_runtime_loop_init(void) {
    AstraResult bus_result = astra_event_bus_init();
    if(bus_result.status != AstraStatusOk) {
        return bus_result;
    }

    AstraResult scheduler_result = astra_scheduler_init();
    if(scheduler_result.status != AstraStatusOk) {
        return scheduler_result;
    }

    astra_runtime_loop_initialized = true;
    return astra_result_ok();
}

AstraResult astra_runtime_loop_step(void) {
    if(!astra_runtime_loop_initialized) {
        return astra_result_error(AstraStatusInvalidArgument, "runtime loop is not initialized");
    }

    AstraResult result = astra_scheduler_step();
    if(result.status == AstraStatusNotFound) {
        return astra_result_ok();
    }

    return result;
}

AstraResult astra_runtime_loop_shutdown(void) {
    astra_runtime_loop_initialized = false;
    return astra_result_ok();
}
