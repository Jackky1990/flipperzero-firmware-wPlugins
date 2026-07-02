#include "screen_manager.h"

#include "app_state.h"
#include "runtime_controller.h"

#include <stdio.h>

AstraStatus astraeon_demo_runtime_controller_request_gpio_write(
    AstraeonRuntimeContext* runtime,
    Storage* storage);
AstraStatus astraeon_demo_runtime_controller_cancel_gpio_write(AstraeonRuntimeContext* runtime);

static void astraeon_screen_status_draw(Canvas* canvas, void* context) {
    AstraeonDemo* app = context;
    AstraeonRuntimeContext* runtime = &app->app.runtime;

    char line[32];

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 25, AlignCenter, AlignCenter, "Runtime Demo");

    snprintf(
        line,
        sizeof(line),
        "Runtime: %s",
        runtime->ready ? "READY" : "IDLE");
    canvas_draw_str_aligned(canvas, 64, 37, AlignCenter, AlignCenter, line);

    snprintf(
        line,
        sizeof(line),
        "Transport: %s",
        runtime->transport_ready ? "LOOPBACK" : "WAIT");
    canvas_draw_str_aligned(canvas, 64, 49, AlignCenter, AlignCenter, line);

    snprintf(
        line,
        sizeof(line),
        "Ping:%s GPIO:%s",
        runtime->ping_ok ? "PASS" : "WAIT",
        runtime->gpio_adapter_bound ? "BIND" : "WAIT");
    canvas_draw_str_aligned(canvas, 64, 61, AlignCenter, AlignCenter, line);
}

static bool astraeon_screen_status_input(InputEvent* event, void* context) {
    if(event->type != InputTypeShort || event->key != InputKeyOk) {
        return false;
    }

    AstraeonDemo* app = context;
    astraeon_demo_runtime_controller_start(&app->app.runtime, app->app.platform.storage);
    return true;
}

static void astraeon_screen_gpio_draw(Canvas* canvas, void* context) {
    AstraeonDemo* app = context;
    AstraeonRuntimeContext* runtime = &app->app.runtime;
    char line[32];

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 12, AlignCenter, AlignCenter, "GPIO PC0");

    snprintf(
        line,
        sizeof(line),
        "Read:%s %s",
        runtime->gpio_read_checked ? (runtime->gpio_read_ok ? "OK" : "SAFE") : "WAIT",
        runtime->gpio_read_ok ? (runtime->gpio_read_value ? "HIGH" : "LOW") : "--");
    canvas_draw_str_aligned(canvas, 64, 25, AlignCenter, AlignCenter, line);

    const char* write_status = "WAIT";
    if(runtime->gpio_write_confirm_required) {
        write_status = "CONFIRM";
    } else if(runtime->gpio_write_session_active) {
        write_status = "BUSY";
    } else if(runtime->gpio_write_checked) {
        write_status = runtime->gpio_write_ok ? "OK" : "SAFE";
    }

    snprintf(line, sizeof(line), "Write:%s", write_status);
    canvas_draw_str_aligned(canvas, 64, 38, AlignCenter, AlignCenter, line);

    snprintf(
        line,
        sizeof(line),
        "Restore:%u",
        (unsigned)runtime->gpio_write_restore_count);
    canvas_draw_str_aligned(canvas, 64, 51, AlignCenter, AlignCenter, line);

    canvas_draw_str_aligned(
        canvas,
        64,
        63,
        AlignCenter,
        AlignCenter,
        runtime->gpio_write_confirm_required ? "OK to write" : "OK to arm");
}

static bool astraeon_screen_gpio_input(InputEvent* event, void* context) {
    AstraeonDemo* app = context;

    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        if(!app->app.runtime.gpio_write_confirm_required &&
           !app->app.runtime.gpio_write_session_active) {
            return false;
        }

        astraeon_demo_runtime_controller_cancel_gpio_write(&app->app.runtime);
        return true;
    }

    if(event->type != InputTypeShort || event->key != InputKeyOk) {
        return false;
    }

    astraeon_demo_runtime_controller_request_gpio_write(
        &app->app.runtime,
        app->app.platform.storage);
    return true;
}

static void astraeon_screen_uart_draw(Canvas* canvas, void* context) {
    AstraeonDemo* app = context;
    AstraeonRuntimeContext* runtime = &app->app.runtime;
    char line[32];

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 12, AlignCenter, AlignCenter, "UART TX");

    snprintf(
        line,
        sizeof(line),
        "Session:%s",
        runtime->uart_checked ? (runtime->uart_ok ? "OK" : "SAFE") : "WAIT");
    canvas_draw_str_aligned(canvas, 64, 25, AlignCenter, AlignCenter, line);

    snprintf(
        line,
        sizeof(line),
        "TX:%s %lu",
        runtime->uart_tx_checked ? (runtime->uart_tx_ok ? "OK" : "SAFE") : "WAIT",
        (unsigned long)runtime->uart_tx_bytes_written);
    canvas_draw_str_aligned(canvas, 64, 38, AlignCenter, AlignCenter, line);

    snprintf(
        line,
        sizeof(line),
        "Release:%u",
        (unsigned)runtime->uart_release_count);
    canvas_draw_str_aligned(canvas, 64, 51, AlignCenter, AlignCenter, line);

    canvas_draw_str_aligned(canvas, 64, 63, AlignCenter, AlignCenter, "OK to TX");
}

static bool astraeon_screen_uart_input(InputEvent* event, void* context) {
    if(event->type != InputTypeShort || event->key != InputKeyOk) {
        return false;
    }

    static const uint8_t payload[] = "ASTRAEON UART TEST 001";

    AstraeonDemo* app = context;
    AstraFlipperSerialAdapter adapter;
    AstraDeviceSerialConfig config = {
        .channel = AstraDeviceSerialChannelPrimary,
        .baud_rate = 115200,
        .data_bits = 8,
        .parity = AstraDeviceSerialParityNone,
        .stop_bits = AstraDeviceSerialStopBits1,
        .flow_control = AstraDeviceSerialFlowControlNone,
        .rx_buffer_size = 256,
        .tx_buffer_size = 256,
        .timeout_ms = 100,
    };

    AstraStatus status = astraeon_demo_runtime_controller_open_uart(
        &app->app.runtime,
        &adapter,
        app->app.platform.storage,
        &config);

    if(status == AstraStatusOk) {
        size_t written = 0;
        astraeon_demo_runtime_controller_uart_tx(
            &app->app.runtime,
            &adapter,
            app->app.platform.storage,
            payload,
            sizeof(payload) - 1,
            &written);
    }

    if(app->app.runtime.uart_session_active) {
        astraeon_demo_runtime_controller_close_uart(
            &app->app.runtime,
            &adapter,
            app->app.platform.storage);
    }

    return true;
}

static const AstraeonScreenInfo astraeon_screens[] = {
    {AstraeonScreenStatus, "Status", astraeon_screen_status_draw, astraeon_screen_status_input},
    {AstraeonScreenGpio, "GPIO", astraeon_screen_gpio_draw, astraeon_screen_gpio_input},
    {AstraeonScreenUart, "UART", astraeon_screen_uart_draw, astraeon_screen_uart_input},
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
