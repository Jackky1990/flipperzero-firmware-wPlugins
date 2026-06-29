#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t menu_index;
    uint8_t current_screen;
    bool redraw;
} AstraUiContext;
