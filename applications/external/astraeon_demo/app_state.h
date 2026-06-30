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
} AstraeonDemo;
