#include "astra_event_types.h"

const char* astra_event_category_to_string(AstraEventCategory category) {
    switch(category) {
    case AstraEventCategorySystem:
        return "SYSTEM";
    case AstraEventCategoryTransport:
        return "TRANSPORT";
    case AstraEventCategoryDevice:
        return "DEVICE";
    case AstraEventCategoryStorage:
        return "STORAGE";
    case AstraEventCategorySecurity:
        return "SECURITY";
    case AstraEventCategoryMission:
        return "MISSION";
    case AstraEventCategoryInventory:
        return "INVENTORY";
    case AstraEventCategoryDiagnostics:
        return "DIAGNOSTICS";
    case AstraEventCategoryAutomation:
        return "AUTOMATION";
    case AstraEventCategoryUi:
        return "UI";
    case AstraEventCategoryUnknown:
        return "UNKNOWN";
    default:
        return "UNKNOWN";
    }
}

const char* astra_event_type_to_string(AstraEventType type) {
    switch(type) {
    case AstraEventTypeNone:
        return "NONE";
    case AstraEventTypeBoot:
        return "BOOT";
    case AstraEventTypeShutdown:
        return "SHUTDOWN";
    case AstraEventTypeHeartbeat:
        return "HEARTBEAT";
    case AstraEventTypeStatus:
        return "STATUS";
    case AstraEventTypeCommand:
        return "COMMAND";
    case AstraEventTypeAck:
        return "ACK";
    case AstraEventTypeError:
        return "ERROR";
    case AstraEventTypeTransportRx:
        return "TRANSPORT_RX";
    case AstraEventTypeTransportTx:
        return "TRANSPORT_TX";
    case AstraEventTypeMissionStart:
        return "MISSION_START";
    case AstraEventTypeMissionComplete:
        return "MISSION_COMPLETE";
    case AstraEventTypeInventoryScan:
        return "INVENTORY_SCAN";
    case AstraEventTypeDiagnosticsReport:
        return "DIAGNOSTICS_REPORT";
    case AstraEventTypeAutomationTrigger:
        return "AUTOMATION_TRIGGER";
    case AstraEventTypeUnknown:
        return "UNKNOWN";
    default:
        return "UNKNOWN";
    }
}
