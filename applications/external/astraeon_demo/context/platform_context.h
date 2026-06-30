#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <storage/storage.h>

typedef struct {
    FuriMessageQueue* queue;
    ViewPort* view_port;
    Gui* gui;
    Storage* storage;
    bool running;
} AstraeonPlatformContext;
