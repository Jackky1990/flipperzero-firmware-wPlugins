#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include "context/app_context.h"

typedef struct {
    AstraeonApplication app;
    FuriMessageQueue* queue;
    ViewPort* view_port;
    Gui* gui;
    bool running;
    bool runtime_ready;
    bool transport_ready;
    bool ping_pass;
    uint8_t menu_index;
} AstraeonDemo;
