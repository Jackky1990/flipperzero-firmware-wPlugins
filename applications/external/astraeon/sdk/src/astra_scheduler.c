#include "astra_scheduler.h"
#include "astra_event_queue.h"
#include "astra_event_bus.h"
#include "astra_runtime_default.h"

static AstraEventQueue* astra_scheduler_queue_from_context(AstraRuntimeContext* context) {
    if(!context) {
        return 0;
    }

    return (AstraEventQueue*)astra_runtime_context_get_scheduler(context);
}

static AstraResult astra_scheduler_queue_init(AstraEventQueue* queue) {
    if(!queue) {
        return astra_result_error(AstraStatusInvalidArgument, "scheduler queue is null");
    }

    return astra_event_queue_init(queue);
}

static AstraResult astra_scheduler_queue_schedule(AstraEventQueue* queue, const AstraEvent* event) {
    if(!queue) {
        return astra_result_error(AstraStatusInvalidArgument, "scheduler queue is null");
    }

    return astra_event_queue_push(queue, event);
}

static AstraResult astra_scheduler_queue_step(AstraRuntimeContext* context, AstraEventQueue* queue) {
    AstraEvent event;

    if(!queue) {
        return astra_result_error(AstraStatusInvalidArgument, "scheduler queue is null");
    }

    AstraResult pop_result = astra_event_queue_pop(queue, &event);
    if(pop_result.status != AstraStatusOk) {
        return pop_result;
    }

    return astra_event_bus_publish_context(context, &event);
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

    AstraResult init_result = astra_scheduler_queue_init(&context->scheduler_queue);
    if(init_result.status != AstraStatusOk) {
        return init_result;
    }

    return astra_runtime_context_set_scheduler(context, &context->scheduler_queue);
}

AstraResult astra_scheduler_schedule_context(AstraRuntimeContext* context, const AstraEvent* event) {
    return astra_scheduler_queue_schedule(astra_scheduler_queue_from_context(context), event);
}

AstraResult astra_scheduler_step_context(AstraRuntimeContext* context) {
    return astra_scheduler_queue_step(context, astra_scheduler_queue_from_context(context));
}
