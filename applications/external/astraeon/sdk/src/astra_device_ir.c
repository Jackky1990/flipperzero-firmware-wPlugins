#include "astra_device_ir.h"

AstraResult astra_device_ir_validate(const AstraDeviceIR* ir) {
    if(!ir) {
        return astra_result_error(AstraStatusInvalidArgument, "ir is null");
    }

    return astra_result_ok();
}
