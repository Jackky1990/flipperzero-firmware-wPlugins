#include "screen_manager.h"

const char* astraeon_screen_title(AstraeonScreen screen) {
    switch(screen) {
    case AstraeonScreenStatus:
        return "Status";
    case AstraeonScreenGpio:
        return "GPIO";
    case AstraeonScreenNfc:
        return "NFC";
    case AstraeonScreenRfid:
        return "RFID";
    case AstraeonScreenSubGhz:
        return "SubGHz";
    default:
        return "Unknown";
    }
}

uint8_t astraeon_screen_count(void) {
    return 5;
}
