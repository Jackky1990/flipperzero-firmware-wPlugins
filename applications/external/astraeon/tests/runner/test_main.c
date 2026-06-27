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
    run_test("dispatcher", astra_test_dispatcher);
    run_test("registry", astra_test_registry);
    run_test("handler", astra_test_handler);
    run_test("event_lifecycle", astra_test_event_lifecycle);
    run_test("event_bus", astra_test_event_bus);
    run_test("runtime_loop", astra_test_runtime_loop);
    run_test("runtime_pipeline", astra_test_runtime_pipeline);
    run_test("scheduler", astra_test_scheduler);
    run_test("runtime_context", astra_test_runtime_context);
    run_test("event_source", astra_test_event_source);
    run_test("runtime_default", astra_test_runtime_default);

    printf("----------------------\n");
    printf("%d / %d PASSED\n", passed, total);

    return passed == total ? 0 : 1;
}
