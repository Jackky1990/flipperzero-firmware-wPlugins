#include "astra_tests.h"
#include "astra_runtime_loop.h"

bool astra_test_runtime_loop(void) {
    if(astra_runtime_loop_step().status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_loop_init().status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_loop_step().status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_loop_shutdown().status != AstraStatusOk) {
        return false;
    }

    return astra_runtime_loop_step().status == AstraStatusInvalidArgument;
}
