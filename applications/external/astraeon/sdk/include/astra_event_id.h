#pragma once

#include "astra_event_types.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ASTRA_EVENT_ID_PREFIX "EVT-"
#define ASTRA_EVENT_ID_SEQUENCE_DIGITS 8
#define ASTRA_EVENT_ID_LEN 12

AstraStatus astra_event_id_generate(AstraEventId out_id, uint32_t sequence);
bool astra_event_id_is_valid(const char* event_id);

#ifdef __cplusplus
}
#endif
