#include "astra_tests.h"
#include "astra_clock.h"

bool astra_test_clock(void) {
    astra_clock_init();
    return astra_clock_now() == 0;
}
