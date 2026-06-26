#include "astra_tests.h"

bool astra_test_run_all(void) {
    bool ok = true;

    ok = astra_test_clock() && ok;
    ok = astra_test_event_builder() && ok;
    ok = astra_test_event_queue() && ok;

    return ok;
}
