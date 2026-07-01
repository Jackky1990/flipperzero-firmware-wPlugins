#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AstraDeviceGPIOModeInput = 0,
    AstraDeviceGPIOModeOutput,
    AstraDeviceGPIOModeInterrupt,
} AstraDeviceGPIOMode;

typedef struct {
    uint8_t pin;
    AstraDeviceGPIOMode mode;
    bool active_high;
} AstraDeviceGPIOPin;

AstraResult astra_device_gpio_pin_validate(const AstraDeviceGPIOPin* pin);

#ifdef __cplusplus
}
#endif
