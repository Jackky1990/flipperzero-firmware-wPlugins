#include "astra_scheduler.h"
#include "astra_event_queue.h"
#include "astra_event_bus.h"
#include "astra_runtime_default.h"

typedef struct {
    AstraEventQueue queue;
} AstraSchedulerStore;

static AstraSchedulerStore context_scheduler;

static AstraSchedulerStore* astra_scheduler_store_from_context(AstraRuntimeContext* context) {
    if(!context) {
        return 0;
    }

    return (AstraSchedulerStore*)astra_runtime_context_get_scheduler(context);
}

static AstraResult astra_scheduler_store_init(AstraSchedulerStore* store) {
    if(!store) {
        return astra_result_error(AstraStatusInvalidArgument, "scheduler store is null");
    }

    return astra_event_queue_init(&store->queue);
}

static AstraResult astra_scheduler_store_schedule(AstraSchedulerStore* store, const AstraEvent* event) {
    if(!store) {
        return astra_result_error(AstraStatusInvalidArgument, "scheduler store is null");
    }

    return astra_event_queue_push(&store->queue, event);
}

static AstraResult astra_scheduler_store_step(AstraSchedulerStore* store) {
    AstraEvent event;

    if(!store) {
        return astra_result_error(AstraStatusInvalidArgument, "scheduler store is null");
    }

    AstraResult pop_result = astra_event_queue_pop(&store->queue, &event);
    if(pop_result.status != AstraStatusOk) {
        return pop_result;
    }

    return astra_event_bus_publish(&event);
}

AstraResult astra_scheduler_init(void) {
    return astra_scheduler_init_context(astra_runtime_default_context());
}

AstraResult astra_scheduler_schedule(const AstraEvent* event) {
    return astra_scheduler_schedule_context(astra_runtime_default_context(), event);
}

AstraResult astra_scheduler_step(void) {
    return astra_scheduler_step_context(astra_runtime_default_context());
}

AstraResult astra_scheduler_init_context(AstraRuntimeContext* context) {
    if(!context) {
        return astra_result_error(AstraStatusInvalidArgument, "context is null");
    }

    AstraResult init_result = astra_scheduler_store_init(&context_scheduler);
    if(init_result.status != AstraStatusOk) {
        return init_result;
    }

    return astra_runtime_context_set_scheduler(context, &context_scheduler);
}

AstraResult astra_scheduler_schedule_context(AstraRuntimeContext* context, const AstraEvent* event) {
    return astra_scheduler_store_schedule(astra_scheduler_store_from_context(context), event);
}

AstraResult astra_scheduler_step_context(AstraRuntimeContext* context) {
    return astra_scheduler_store_step(astra_scheduler_store_from_context(context));
}
