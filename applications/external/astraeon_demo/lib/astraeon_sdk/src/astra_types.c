#include "astra_types.h"

const char* astra_module_type_to_string(AstraModuleType type) {
    switch(type) {
    case AstraModuleSystem:
        return "SYSTEM";
    case AstraModuleTransport:
        return "TRANSPORT";
    case AstraModuleStorage:
        return "STORAGE";
    case AstraModuleLogger:
        return "LOGGER";
    case AstraModuleEvent:
        return "EVENT";
    case AstraModulePolicy:
        return "POLICY";
    case AstraModuleMission:
        return "MISSION";
    case AstraModuleInventory:
        return "INVENTORY";
    case AstraModuleDiagnostics:
        return "DIAGNOSTICS";
    case AstraModuleAutomation:
        return "AUTOMATION";
    case AstraModuleApplication:
        return "APPLICATION";
    case AstraModuleUnknown:
        return "UNKNOWN";
    default:
        return "UNKNOWN";
    }
}
