#pragma once

#include "app_state.h"

#ifdef __cplusplus
extern "C" {
#endif

void astraeon_application_bind(AstraeonDemo* app);
AstraeonDemo* astraeon_application_get(void);

void astraeon_application_stop(void);
bool astraeon_application_is_running(void);

void astraeon_application_request_redraw(void);

#ifdef __cplusplus
}
#endif
