#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ASTRAEON Runtime Configuration
 *
 * Default build keeps hardware transports disabled.
 * Enable USB CDC only in Flipper firmware build path when ready:
 *
 *   #define ASTRA_RUNTIME_ENABLE_USB_CDC 1
 */

#ifndef ASTRA_RUNTIME_ENABLE_USB_CDC
#define ASTRA_RUNTIME_ENABLE_USB_CDC 0
#endif

#ifdef __cplusplus
}
#endif
