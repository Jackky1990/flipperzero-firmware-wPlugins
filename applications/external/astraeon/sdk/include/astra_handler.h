#pragma once

#include "astra_result.h"
#include "astra_event.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef AstraResult (*AstraEventHandler)(AstraEvent* event);

AstraResult astra_handler_init(void);

AstraResult astra_handler_invoke(AstraEventHandler handler, AstraEvent* event);

#ifdef __cplusplus
}
#endif
