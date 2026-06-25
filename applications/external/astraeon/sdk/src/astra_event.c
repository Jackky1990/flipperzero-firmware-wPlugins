#include "astra_event.h"

const char* astra_event_state_to_string(AstraEventState state) {
    switch(state) {
    case AstraEventStateNew:
        return "NEW";
    case AstraEventStateInitialized:
        return "INITIALIZED";
    case AstraEventStateQueued:
        return "QUEUED";
    case AstraEventStateDispatching:
        return "DISPATCHING";
    case AstraEventStateHandled:
        return "HANDLED";
    case AstraEventStateCompleted:
        return "COMPLETED";
    case AstraEventStateFailed:
        return "FAILED";
    case AstraEventStateUnknown:
        return "UNKNOWN";
    default:
        return "UNKNOWN";
    }
}

const char* astra_event_priority_to_string(AstraEventPriority priority) {
    switch(priority) {
    case AstraEventPriorityLow:
        return "LOW";
    case AstraEventPriorityNormal:
        return "NORMAL";
    case AstraEventPriorityHigh:
        return "HIGH";
    case AstraEventPriorityCritical:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}
