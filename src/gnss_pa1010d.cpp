#include "gnss_pa1010d.h"
#include "app_config.h"
#include <cstring>
#include <cstdlib>

static BufferedSerial gps_uart(GNSS_TX, GNSS_RX, GNSS_BAUD);
static DigitalOut gps_wake(GNSS_WAKE_PIN, 0); // 0 = sleep

static void uart_flush(BufferedSerial& ser) {
    char c;
    while (ser.readable()) { ser.read(&c, 1); }
}

static float conv_degmin_to_deg(const char* degmin, char hemi) {
    if (!degmin || degmin[0] == '\0') return 0.0f;
    float v = atof(degmin);
    int deg = (int)(v / 100.0f);
    float min = v - (float)deg * 100.0f;
    float res = (float)deg + min / 60.0f;
    if (hemi == 'S' || hemi == 'W') res = -res;
    return res;
}

static void parse_gga_line(SystemContext& ctx, const char* line) {
    char tmp[128];
    strncpy(tmp, line, sizeof(tmp));
    tmp[sizeof(tmp)-1] = '\0';

    const char* fields[15] = {0};
    int idx = 0;

    char* p = strtok(tmp, ",");
    while (p && idx < 15) {
        fields[idx++] = p;
        p = strtok(NULL, ",");
    }
    if (idx < 7) return;

    const char* lat_str = fields[2];
    const char* ns_str  = fields[3];
    const char* lon_str = fields[4];
    const char* ew_str  = fields[5];
    const char* fix_str = fields[6];

    int fix = atoi(fix_str);
    ctx.gps_has_fix = (fix > 0);

    if (ctx.gps_has_fix && lat_str && lon_str && ns_str && ew_str &&
        strlen(lat_str) > 0 && strlen(lon_str) > 0) {

        ctx.gps_latitude  = conv_degmin_to_deg(lat_str, ns_str[0]);
        ctx.gps_longitude = conv_degmin_to_deg(lon_str, ew_str[0]);

        if (ctx.gnss_window_active || ctx.state == STATE_ALARM) {
            ctx.stored_lat = ctx.gps_latitude;
            ctx.stored_lon = ctx.gps_longitude;
            ctx.stored_valid = true;
        }
    }
}

static void gnss_set_wake(bool on) {
    gps_wake = on ? 1 : 0;
    printf("[GNSS] WAKE=%d\r\n", (int)gps_wake.read());
}

static void gnss_enter_hw_sleep(SystemContext& ctx) {
    ctx.gnss_uart_poll_enabled = false;
    ctx.gnss_on = false;

    // FIX_NOW reset
    ctx.gps_has_fix  = false;
    ctx.gps_latitude = 0.0f;
    ctx.gps_longitude= 0.0f;

    if (!GNSS_HAS_HW_SLEEP) {
        gnss_set_wake(true);
        uart_flush(gps_uart);
        printf("[GNSS] OFF -> poll disabled (no HW sleep)\r\n");
        return;
    }

    // 1) WAKE high so module accepts standby command
    gnss_set_wake(true);
    ThisThread::sleep_for(GNSS_WAKE_SETTLE_TIME);

    // 2) PMTK161 standby
    const char standby_cmd[] = "$PMTK161,0*28\r\n";
    gps_uart.write(standby_cmd, sizeof(standby_cmd)-1);
    ThisThread::sleep_for(GNSS_STANDBY_CMD_WAIT);

    // 3) flush then WAKE low
    uart_flush(gps_uart);

    gnss_set_wake(false);
    ThisThread::sleep_for(GNSS_SLEEP_SETTLE_TIME);

    uart_flush(gps_uart);

    printf("[GNSS] OFF -> PMTK161 + WAKE=0 (HW sleep)\r\n");
}

static void gnss_wake_and_enable_poll(SystemContext& ctx) {
    gnss_set_wake(true);
    ThisThread::sleep_for(GNSS_WAKE_SETTLE_TIME);

    // wake kick 0xFF
    const uint8_t wake_ff = 0xFF;
    gps_uart.write(&wake_ff, 1);
    ThisThread::sleep_for(GNSS_WAKE_KICK_WAIT);

    uart_flush(gps_uart);

    ctx.gnss_on = true;
    ctx.gnss_uart_poll_enabled = true;

    printf("[GNSS] ON (WAKE=1, poll enabled)\r\n");
}

bool gnss_init(SystemContext& ctx) {
    gps_uart.set_baud(GNSS_BAUD);
    gps_uart.set_format(8, BufferedSerial::None, 1);
    gps_uart.set_blocking(false);

    // start in real sleep
    gnss_enter_hw_sleep(ctx);
    return true;
}

void gnss_set_on(SystemContext& ctx, bool on) {
    if (on) gnss_wake_and_enable_poll(ctx);
    else    gnss_enter_hw_sleep(ctx);
}

void gnss_uart_poll(SystemContext& ctx) {
    if (!ctx.gnss_uart_poll_enabled) return;

    static char line[256];
    static size_t pos = 0;

    while (gps_uart.readable()) {
        char c;
        if (gps_uart.read(&c, 1) != 1) break;
        if (c == '\r') continue;

        if (c == '\n') {
            line[pos] = '\0';
            if (strncmp(line, "$GPGGA", 6) == 0 || strncmp(line, "$GNGGA", 6) == 0) {
                parse_gga_line(ctx, line);
            }
            pos = 0;
        } else {
            if (pos < sizeof(line)-1) line[pos++] = c;
            else pos = 0;
        }
    }
}

void gnss_start_window(SystemContext& ctx) {
    if (ctx.state != STATE_MONITORING) return;

    ctx.gnss_window_active = true;
    ctx.gnss_window_deadline = Kernel::Clock::now() + GNSS_WINDOW_LENGTH;

    gnss_set_on(ctx, true);
    printf("[GNSS] window START\r\n");
}

void gnss_stop_window(SystemContext& ctx) {
    ctx.gnss_window_active = false;
    gnss_set_on(ctx, false);
    printf("[GNSS] window STOP\r\n");
}
