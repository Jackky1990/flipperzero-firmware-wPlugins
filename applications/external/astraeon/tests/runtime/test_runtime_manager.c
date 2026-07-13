#include "astra_tests.h"
#include "astra_runtime_manager.h"

bool astra_test_runtime_manager(void) {
    AstraRuntimeId id1 = 0;
    AstraRuntimeId id2 = 0;
    AstraRuntimeId id3 = 0;
    AstraRuntimeId id4 = 0;

    AstraRuntimeContext* ctx1 = 0;
    AstraRuntimeContext* ctx2 = 0;
    AstraRuntimeContext* ctx3 = 0;
    AstraRuntimeContext* ctx4 = 0;

    if(astra_runtime_manager_init().status != AstraStatusOk) {
        return false;
    }

    if(!astra_runtime_default()) {
        return false;
    }

    if(!astra_runtime_get(0)) {
        return false;
    }

    AstraRuntimeContext* default_context = astra_runtime_default();
    AstraRuntimeContext* manager_default_context = astra_runtime_get(0);

    if(default_context != manager_default_context) {
        return false;
    }

    default_context->queue = default_context;
    if(manager_default_context->queue != default_context) {
        return false;
    }
    default_context->queue = 0;

    if(astra_runtime_current_id() != 0) {
        return false;
    }

    if(astra_runtime_current() != astra_runtime_get(0)) {
        return false;
    }

    if(astra_runtime_current_id() != 0) {
        return false;
    }

    if(astra_runtime_current() != astra_runtime_get(0)) {
        return false;
    }

    if(astra_runtime_count() != 1) {
        return false;
    }

    if(!astra_runtime_exists(0)) {
        return false;
    }

    if(astra_runtime_exists(999)) {
        return false;
    }

    if(astra_runtime_create(0, &ctx1).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_create(&id1, 0).status != AstraStatusInvalidArgument) {
        return false;
    }

    if(astra_runtime_create(&id1, &ctx1).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_count() != 2) {
        return false;
    }

    if(astra_runtime_create(&id2, &ctx2).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_count() != 3) {
        return false;
    }

    if(astra_runtime_create(&id3, &ctx3).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_count() != 4) {
        return false;
    }

    if(id1 == 0 || id2 == 0 || id3 == 0) {
        return false;
    }

    if(id1 == id2 || id1 == id3 || id2 == id3) {
        return false;
    }

    if(!ctx1 || !ctx2 || !ctx3) {
        return false;
    }

    if(!ctx1->initialized || !ctx2->initialized || !ctx3->initialized) {
        return false;
    }

    if(astra_runtime_get(id1) != ctx1) {
        return false;
    }

    if(astra_runtime_set_current(id1).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_current_id() != id1) {
        return false;
    }

    if(astra_runtime_current() != ctx1) {
        return false;
    }

    if(astra_runtime_set_current(999).status != AstraStatusNotFound) {
        return false;
    }

    if(astra_runtime_current_id() != id1) {
        return false;
    }

    if(astra_runtime_set_current(id1).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_current_id() != id1) {
        return false;
    }

    if(astra_runtime_current() != ctx1) {
        return false;
    }

    if(astra_runtime_set_current(999).status != AstraStatusNotFound) {
        return false;
    }

    if(astra_runtime_current_id() != id1) {
        return false;
    }

    if(astra_runtime_get(id2) != ctx2) {
        return false;
    }

    if(astra_runtime_get(id3) != ctx3) {
        return false;
    }

    if(astra_runtime_create(&id4, &ctx4).status != AstraStatusBusy) {
        return false;
    }

    if(astra_runtime_destroy(id2).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_get(id2) != 0) {
        return false;
    }

    if(astra_runtime_exists(id2)) {
        return false;
    }

    if(astra_runtime_count() != 3) {
        return false;
    }

    if(astra_runtime_create(&id4, &ctx4).status != AstraStatusOk) {
        return false;
    }

    if(id4 != id2) {
        return false;
    }

    if(!ctx4 || !ctx4->initialized) {
        return false;
    }

    if(astra_runtime_count() != 4) {
        return false;
    }

    if(!astra_runtime_exists(id4)) {
        return false;
    }

    if(astra_runtime_set_current(id4).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_set_current(id4).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_destroy(id4).status != AstraStatusOk) {
        return false;
    }

    if(astra_runtime_current() != 0) {
        return false;
    }

    if(astra_runtime_current() != 0) {
        return false;
    }

    if(astra_runtime_destroy(id4).status != AstraStatusNotFound) {
        return false;
    }

    if(astra_runtime_get(999) != 0) {
        return false;
    }

    if(astra_runtime_destroy(0).status != AstraStatusNotFound) {
        return false;
    }

    return true;
}
