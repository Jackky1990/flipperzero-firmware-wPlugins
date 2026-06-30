#include "view.h"
#include "app_state.h"
#include "screen/screen_manager.h"

void astraeon_demo_draw(Canvas* canvas, void* context) {
    AstraeonDemo* app = context;
    AstraeonScreen current_screen = (AstraeonScreen)app->app.ui.current_screen;

    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignCenter, "ASTRAEON-X");

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(
        canvas,
        64,
        20,
        AlignCenter,
        AlignCenter,
        astraeon_screen_title(current_screen));

    astraeon_screen_draw(current_screen, canvas, app);
}
