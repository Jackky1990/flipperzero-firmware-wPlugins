#include "view.h"
#include "app_state.h"
#include "screen/screen_manager.h"

#include <stdio.h>

void astraeon_demo_draw(Canvas* canvas, void* context) {
    AstraeonDemo* app = context;
    AstraeonRuntimeContext* runtime = &app->app.runtime;

    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignCenter, "ASTRAEON-X");

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(
        canvas,
        64,
        21,
        AlignCenter,
        AlignCenter,
        runtime->ping_ok ? "Runtime: READY" : "Runtime: FAIL");

    char status[32];
    snprintf(
        status,
        sizeof(status),
        "P%u %s %s",
        runtime->protocol_version,
        runtime->supports_packets ? "PKT" : "RAW",
        runtime->heartbeat_ok ? "HB" : "NOHB");
    canvas_draw_str_aligned(canvas, 64, 31, AlignCenter, AlignCenter, status);

    uint8_t count = astraeon_screen_count();
    for(uint8_t i = 0; i < count; i++) {
        char line[32];
        snprintf(
            line,
            sizeof(line),
            "%c %s",
            app->app.ui.current_screen == i ? '>' : ' ',
            astraeon_screen_title((AstraeonScreen)i));
        canvas_draw_str(canvas, 22, 39 + (i * 6), line);
    }
}
