#include "astra_flipper_gpio_adapter.h"

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
