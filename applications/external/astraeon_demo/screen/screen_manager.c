#include "screen_manager.h"

typedef struct {
    AstraeonScreen id;
    const char* title;
} AstraeonScreenInfo;

static const AstraeonScreenInfo astraeon_screens[] = {
    {AstraeonScreenStatus, "Status"},
    {AstraeonScreenGpio, "GPIO"},
    {AstraeonScreenNfc, "NFC"},
    {AstraeonScreenRfid, "RFID"},
    {AstraeonScreenSubGhz, "SubGHz"},
};

const char* astraeon_screen_title(AstraeonScreen screen) {
    for(uint8_t i = 0; i < astraeon_screen_count(); i++) {
        if(astraeon_screens[i].id == screen) {
            return astraeon_screens[i].title;
        }
    }

    return "Unknown";
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
