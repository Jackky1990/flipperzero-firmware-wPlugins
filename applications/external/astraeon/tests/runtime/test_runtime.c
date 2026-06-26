#include "astra_tests.h"
#include "astra_runtime.h"

bool astra_test_runtime(void) {
    AstraResult result = astra_runtime_init();
    if(result.status != AstraStatusOk) {
        return false;
    }

    result = astra_runtime_step();
    if(result.status != AstraStatusOk) {
        return false;
    }

    return true;
}
