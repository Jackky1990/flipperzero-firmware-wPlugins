#include "view.h"
#include "app_state.h"
#include "screen/screen_manager.h"

#include <stdio.h>

void astraeon_demo_draw(Canvas* canvas, void* context) {
    AstraeonDemo* app = context;

    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignCenter, "ASTRAEON-X");

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 23, AlignCenter, AlignCenter, app->ping_pass ? "Runtime: READY" : "Runtime: FAIL");

    uint8_t count = astraeon_screen_count();
    for(uint8_t i = 0; i < count; i++) {
        char line[32];
        snprintf(
            line,
            sizeof(line),
            "%c %s",
            app->app.ui.current_screen == i ? '>' : ' ',
            astraeon_screen_title((AstraeonScreen)i));
        canvas_draw_str(canvas, 22, 36 + (i * 9), line);
    }
}
