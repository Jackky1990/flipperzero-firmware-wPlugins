#pragma once

#include "app_state.h"
#include "context/app_context.h"

#ifdef __cplusplus
extern "C" {
#endif

void astraeon_application_bind(AstraeonDemo* app);
AstraeonDemo* astraeon_application_get(void);

AstraeonRuntimeContext* astraeon_application_runtime(void);
AstraeonUiContext* astraeon_application_ui(void);
AstraeonPlatformContext* astraeon_application_platform(void);

void astraeon_application_stop(void);
bool astraeon_application_is_running(void);

void astraeon_application_request_redraw(void);

#ifdef __cplusplus
}
#endif
