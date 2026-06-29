#pragma once

#include <furi.h>
#include <gui/gui.h>

typedef struct {
    FuriMessageQueue* queue;
    ViewPort* view_port;
    Gui* gui;
    bool running;
} AstraPlatformContext;
