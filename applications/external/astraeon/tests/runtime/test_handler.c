#include "astra_tests.h"
#include "astra_handler.h"

static bool handler_called = false;

static AstraResult astra_test_handler_ok(AstraEvent* event) {
    if(!event) {
        return astra_result_error(AstraStatusInvalidArgument, "event is null");
    }

    handler_called = true;
    return astra_result_ok();
}

bool astra_test_handler(void) {
    AstraEvent event;
    handler_called = false;

    if(astra_handler_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_handler_invoke(0, &event).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_handler_invoke(astra_test_handler_ok, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_handler_invoke(astra_test_handler_ok, &event).status != AstraStatusOk) {
        return false;
    }

    return handler_called;
}
