#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AstraeonScreenStatus = 0,
    AstraeonScreenGpio,
    AstraeonScreenNfc,
    AstraeonScreenRfid,
    AstraeonScreenSubGhz,
} AstraeonScreen;

const char* astraeon_screen_title(AstraeonScreen screen);
uint8_t astraeon_screen_count(void);
AstraeonScreen astraeon_screen_next(AstraeonScreen screen);
AstraeonScreen astraeon_screen_previous(AstraeonScreen screen);

#ifdef __cplusplus
}
#endif
