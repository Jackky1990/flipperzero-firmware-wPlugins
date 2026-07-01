#pragma once

#include <stddef.h>

#include "astra_device_gpio.h"
#include "astra_result.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AstraFlipperGPIOPinPC0 = 0,
    AstraFlipperGPIOPinPC1,
    AstraFlipperGPIOPinPC3,
    AstraFlipperGPIOPinPB2,
    AstraFlipperGPIOPinPB3,
    AstraFlipperGPIOPinPA4,
    AstraFlipperGPIOPinPA6,
    AstraFlipperGPIOPinPA7,
    AstraFlipperGPIOPinCount,
} AstraFlipperGPIOPinId;

typedef struct {
    AstraFlipperGPIOPinId id;
    AstraDeviceGPIOPin contract;
    const void* flipper_pin;
} AstraFlipperGPIOPinBinding;

typedef struct {
    AstraFlipperGPIOPinBinding pins[AstraFlipperGPIOPinCount];
    size_t pin_count;
} AstraFlipperGPIOAdapter;

AstraResult astra_flipper_gpio_adapter_init(AstraFlipperGPIOAdapter* adapter);

AstraResult astra_flipper_gpio_adapter_bind_resources(AstraFlipperGPIOAdapter* adapter);

AstraResult astra_flipper_gpio_adapter_validate(const AstraFlipperGPIOAdapter* adapter);

size_t astra_flipper_gpio_adapter_pin_count(const AstraFlipperGPIOAdapter* adapter);

const AstraFlipperGPIOPinBinding* astra_flipper_gpio_adapter_pin_at(
    const AstraFlipperGPIOAdapter* adapter,
    size_t index);

AstraResult astra_flipper_gpio_adapter_read_pin(
    const AstraFlipperGPIOAdapter* adapter,
    AstraFlipperGPIOPinId pin,
    bool* out_value);

#ifdef __cplusplus
}
#endif
