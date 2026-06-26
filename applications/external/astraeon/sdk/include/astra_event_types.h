#pragma once

#include "astra_types.h"
#include "astra_status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AstraEventCategorySystem = 0,
    AstraEventCategoryTransport,
    AstraEventCategoryDevice,
    AstraEventCategoryStorage,
    AstraEventCategorySecurity,
    AstraEventCategoryMission,
    AstraEventCategoryInventory,
    AstraEventCategoryDiagnostics,
    AstraEventCategoryAutomation,
    AstraEventCategoryUi,
    AstraEventCategoryUnknown,
} AstraEventCategory;

typedef enum {
    AstraEventTypeNone = 0,
    AstraEventTypeBoot,
    AstraEventTypeShutdown,
    AstraEventTypeHeartbeat,
    AstraEventTypeStatus,
    AstraEventTypeCommand,
    AstraEventTypeAck,
    AstraEventTypeError,
    AstraEventTypeTransportRx,
    AstraEventTypeTransportTx,
    AstraEventTypeMissionStart,
    AstraEventTypeMissionComplete,
    AstraEventTypeInventoryScan,
    AstraEventTypeDiagnosticsReport,
    AstraEventTypeAutomationTrigger,
    AstraEventTypeUnknown,
} AstraEventType;

const char* astra_event_category_to_string(AstraEventCategory category);
const char* astra_event_type_to_string(AstraEventType type);

#ifdef __cplusplus
}
#endif
