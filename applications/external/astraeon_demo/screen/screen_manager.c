#include "screen_manager.h"

#include "app_state.h"
#include "runtime_controller.h"

#include <stdio.h>

static void astraeon_screen_status_draw(Canvas* canvas, void* context) {
    AstraeonDemo* app = context;
    AstraeonRuntimeContext* runtime = &app->app.runtime;

    char line[32];

    snprintf(
        line,
        sizeof(line),
        "Run:%s Xport:%s",
        runtime->ready ? "OK" : "--",
        runtime->transport_ready ? "OK" : "--");
    canvas_draw_str(canvas, 2, 28, line);

    snprintf(
        line,
        sizeof(line),
        "Proto:P%u %s",
        runtime->protocol_version,
        runtime->supports_packets ? "PKT" : "RAW");
    canvas_draw_str(canvas, 2, 37, line);

    snprintf(
        line,
        sizeof(line),
        "Cap:%s HS:%s Ping:%s",
        runtime->capabilities_ok ? "OK" : "--",
        runtime->handshake_ok ? "OK" : "--",
        runtime->ping_ok ? "OK" : "--");
    canvas_draw_str(canvas, 2, 46, line);

    snprintf(
        line,
        sizeof(line),
        "S%s L%s E%s N%s U%s P%s",
        runtime->storage_ok ? "+" : "-",
        runtime->logger_ok ? "+" : "-",
        runtime->event_persistence_ok ? "+" : "-",
        runtime->node01_ok ? "+" : "-",
        runtime->node01_usb_ok ? "+" : "-",
        runtime->policy_ok ? "+" : "-");
    canvas_draw_str(canvas, 2, 55, line);
}

static bool astraeon_screen_status_input(InputEvent* event, void* context) {
    if(event->type != InputTypeShort || event->key != InputKeyOk) {
        return false;
    }

    AstraeonDemo* app = context;
    astraeon_demo_runtime_controller_start(&app->app.runtime, app->app.platform.storage);
    return true;
}

static const AstraeonScreenInfo astraeon_screens[] = {
    {AstraeonScreenStatus, "Status", astraeon_screen_status_draw, astraeon_screen_status_input},
    {AstraeonScreenGpio, "GPIO", 0, 0},
    {AstraeonScreenNfc, "NFC", 0, 0},
    {AstraeonScreenRfid, "RFID", 0, 0},
    {AstraeonScreenSubGhz, "SubGHz", 0, 0},
};

const AstraeonScreenInfo* astraeon_screen_info(AstraeonScreen screen) {
    for(uint8_t i = 0; i < astraeon_screen_count(); i++) {
        if(astraeon_screens[i].id == screen) {
            return &astraeon_screens[i];
        }
    }

    return 0;
}

const char* astraeon_screen_title(AstraeonScreen screen) {
    const AstraeonScreenInfo* info = astraeon_screen_info(screen);
    return info ? info->title : "Unknown";
}

void astraeon_screen_draw(AstraeonScreen screen, Canvas* canvas, void* context) {
    const AstraeonScreenInfo* info = astraeon_screen_info(screen);

    if(info && info->draw) {
        info->draw(canvas, context);
        return;
    }

    (void)context;
    canvas_draw_str_aligned(canvas, 64, 38, AlignCenter, AlignCenter, "Diagnostics pending");
}

bool astraeon_screen_handle_input(AstraeonScreen screen, InputEvent* event, void* context) {
    const AstraeonScreenInfo* info = astraeon_screen_info(screen);

    if(info && info->input) {
        return info->input(event, context);
    }

    return false;
}

uint8_t astraeon_screen_count(void) {
    return sizeof(astraeon_screens) / sizeof(astraeon_screens[0]);
}

AstraeonScreen astraeon_screen_next(AstraeonScreen screen) {
    uint8_t count = astraeon_screen_count();

    for(uint8_t i = 0; i < count; i++) {
        if(astraeon_screens[i].id == screen) {
            return astraeon_screens[(i + 1) % count].id;
        }
    }

    return AstraeonScreenStatus;
}

AstraeonScreen astraeon_screen_previous(AstraeonScreen screen) {
    uint8_t count = astraeon_screen_count();

    for(uint8_t i = 0; i < count; i++) {
        if(astraeon_screens[i].id == screen) {
            return astraeon_screens[(i + count - 1) % count].id;
        }
    }

    return AstraeonScreenStatus;
}
