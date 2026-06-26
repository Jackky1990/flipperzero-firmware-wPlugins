#include "astra_registry.h"

AstraResult astra_registry_init(void) {
    return astra_result_ok();
}

AstraResult astra_registry_register(AstraEventType type, AstraEventHandler handler) {
    (void)type;
    (void)handler;
    return astra_result_ok();
}

AstraEventHandler astra_registry_find(AstraEventType type) {
    (void)type;
    return 0;
}
