#include "astra_tests.h"
#include <stdio.h>

static int passed = 0;
static int total = 0;

static void run_test(const char* name, bool (*test_fn)(void)) {
    total++;
    bool ok = test_fn();
    if(ok) passed++;
    printf("%-24s %s\n", name, ok ? "PASS" : "FAIL");
}

int main(void) {
    printf("ASTRAEON Runtime Tests\n");
    printf("----------------------\n");

    run_test("clock", astra_test_clock);
    run_test("event_builder", astra_test_event_builder);
    run_test("event_queue", astra_test_event_queue);
    run_test("runtime", astra_test_runtime);

    printf("----------------------\n");
    printf("%d / %d PASSED\n", passed, total);

    return passed == total ? 0 : 1;
}
