#include "screen_manager.h"

static const AstraeonScreenInfo astraeon_screens[] = {
    {AstraeonScreenStatus, "Status", 0, 0},
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
