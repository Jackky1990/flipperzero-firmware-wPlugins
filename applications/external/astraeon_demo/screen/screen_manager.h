#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <gui/gui.h>
#include <input/input.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AstraeonScreenStatus = 0,
    AstraeonScreenGpio,
    AstraeonScreenUart,
    AstraeonScreenNfc,
    AstraeonScreenRfid,
    AstraeonScreenSubGhz,
} AstraeonScreen;

typedef void (*AstraeonScreenDrawCallback)(Canvas* canvas, void* context);
typedef bool (*AstraeonScreenInputCallback)(InputEvent* event, void* context);

typedef struct {
    AstraeonScreen id;
    const char* title;
    AstraeonScreenDrawCallback draw;
    AstraeonScreenInputCallback input;
} AstraeonScreenInfo;

const AstraeonScreenInfo* astraeon_screen_info(AstraeonScreen screen);
const char* astraeon_screen_title(AstraeonScreen screen);
void astraeon_screen_draw(AstraeonScreen screen, Canvas* canvas, void* context);
bool astraeon_screen_handle_input(AstraeonScreen screen, InputEvent* event, void* context);
uint8_t astraeon_screen_count(void);
AstraeonScreen astraeon_screen_next(AstraeonScreen screen);
AstraeonScreen astraeon_screen_previous(AstraeonScreen screen);

#ifdef __cplusplus
}
#endif
