#pragma once

#include <stdbool.h>

typedef struct {
    bool ready;
    bool transport_ready;
    bool ping_ok;
} AstraeonRuntimeContext;
