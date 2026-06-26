#pragma once

#include "astra_result.h"
#include "astra_event.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef AstraResult (*AstraEventHandler)(AstraEvent* event);

AstraResult astra_registry_init(void);

AstraResult astra_registry_register(AstraEventType type, AstraEventHandler handler);

AstraEventHandler astra_registry_find(AstraEventType type);

#ifdef __cplusplus
}
#endif
