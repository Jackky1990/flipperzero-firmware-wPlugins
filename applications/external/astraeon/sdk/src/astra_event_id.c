#include "astra_event_id.h"
#include <stdio.h>
#include <string.h>

static bool astra_event_id_is_digit(char value) {
    return value >= '0' && value <= '9';
}

AstraStatus astra_event_id_generate(AstraEventId out_id, uint32_t sequence) {
    if(!out_id) {
        return AstraStatusInvalidArgument;
    }

    int written = snprintf(out_id, ASTRA_ID_MAX_LEN, "%s%08lu", ASTRA_EVENT_ID_PREFIX, (unsigned long)sequence);
    if(written != ASTRA_EVENT_ID_LEN) {
        out_id[0] = 0;
        return AstraStatusError;
    }

    return AstraStatusOk;
}

bool astra_event_id_is_valid(const char* event_id) {
    if(!event_id) {
        return false;
    }

    if(strlen(event_id) != ASTRA_EVENT_ID_LEN) {
        return false;
    }

    if(strncmp(event_id, ASTRA_EVENT_ID_PREFIX, 4) != 0) {
        return false;
    }

    for(unsigned int index = 4; index < ASTRA_EVENT_ID_LEN; index++) {
        if(!astra_event_id_is_digit(event_id[index])) {
            return false;
        }
    }

    return true;
}
