#include "astra_flipper_gpio_adapter.h"

#if defined(__has_include)
#if __has_include(<furi_hal_resources.h>)
#define ASTRA_FLIPPER_GPIO_HAS_RESOURCES 1
#include <furi_hal_resources.h>
#endif
#endif

static AstraDeviceGPIOPin astra_flipper_gpio_contract(uint8_t pin) {
    AstraDeviceGPIOPin contract = {
        .pin = pin,
        .mode = AstraDeviceGPIOModeInput,
        .active_high = true,
    };
    return contract;
}

AstraResult astra_flipper_gpio_adapter_init(AstraFlipperGPIOAdapter* adapter) {
    if(!adapter) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper gpio adapter is null");
    }

    adapter->pin_count = AstraFlipperGPIOPinCount;

    for(size_t index = 0; index < adapter->pin_count; ++index) {
        adapter->pins[index].id = (AstraFlipperGPIOPinId)index;
        adapter->pins[index].contract = astra_flipper_gpio_contract((uint8_t)index);
        adapter->pins[index].flipper_pin = 0;
    }

    return astra_result_ok();
}

AstraResult astra_flipper_gpio_adapter_bind_resources(AstraFlipperGPIOAdapter* adapter) {
    if(!adapter) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper gpio adapter is null");
    }

    AstraResult result = astra_flipper_gpio_adapter_validate(adapter);
    if(result.status != AstraStatusOk) {
        return result;
    }

#if defined(ASTRA_FLIPPER_GPIO_HAS_RESOURCES)
    static const void* const flipper_gpio_resources[AstraFlipperGPIOPinCount] = {
        &gpio_ext_pc0,
        &gpio_ext_pc1,
        &gpio_ext_pc3,
        &gpio_ext_pb2,
        &gpio_ext_pb3,
        &gpio_ext_pa4,
        &gpio_ext_pa6,
        &gpio_ext_pa7,
    };

    for(size_t index = 0; index < adapter->pin_count; ++index) {
        adapter->pins[index].flipper_pin = flipper_gpio_resources[index];
    }

    return astra_result_ok();
#else
    return astra_result_error(AstraStatusInternalError, "flipper gpio resources are unavailable");
#endif
}

AstraResult astra_flipper_gpio_adapter_validate(const AstraFlipperGPIOAdapter* adapter) {
    if(!adapter) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper gpio adapter is null");
    }

    if(adapter->pin_count != AstraFlipperGPIOPinCount) {
        return astra_result_error(AstraStatusInvalidArgument, "flipper gpio pin count is invalid");
    }

    for(size_t index = 0; index < adapter->pin_count; ++index) {
        const AstraFlipperGPIOPinBinding* binding = &adapter->pins[index];
        if(binding->id != (AstraFlipperGPIOPinId)index) {
            return astra_result_error(AstraStatusInvalidArgument, "flipper gpio pin id is invalid");
        }

        AstraResult result = astra_device_gpio_pin_validate(&binding->contract);
        if(result.status != AstraStatusOk) {
            return result;
        }
    }

    return astra_result_ok();
}

size_t astra_flipper_gpio_adapter_pin_count(const AstraFlipperGPIOAdapter* adapter) {
    if(!adapter) {
        return 0;
    }

    return adapter->pin_count;
}

const AstraFlipperGPIOPinBinding* astra_flipper_gpio_adapter_pin_at(
    const AstraFlipperGPIOAdapter* adapter,
    size_t index) {
    if(!adapter || index >= adapter->pin_count) {
        return 0;
    }

    return &adapter->pins[index];
}
