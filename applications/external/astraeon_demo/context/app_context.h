#pragma once

#include "runtime_context.h"
#include "ui_context.h"
#include "platform_context.h"

typedef struct {
    AstraRuntimeContext runtime;
    AstraUiContext ui;
    AstraPlatformContext platform;
} AstraApplication;
