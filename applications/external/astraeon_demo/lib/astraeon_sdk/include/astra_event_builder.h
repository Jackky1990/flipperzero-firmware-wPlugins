#pragma once

#include "astra_event.h"

#ifdef __cplusplus
extern "C" {
#endif

AstraResult astra_event_builder_init(AstraEvent* event, AstraEventType type);

AstraResult astra_event_builder_reset(AstraEvent* event);

#ifdef __cplusplus
}
#endif
