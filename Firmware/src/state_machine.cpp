#include "state_machine.h"
#include "app_config.h"

// forward hooks (implemented in other modules)
void motion_set_enabled(SystemContext& ctx, bool on);
void gnss_set_on(SystemContext& ctx, bool on);
void gnss_start_window(SystemContext& ctx);
void gnss_stop_window(SystemContext& ctx);

void buzzer_off();
void buzzer_beep_ms(EventQueue& evq, std::chrono::milliseconds ms_on);
void buzzer_start_alarm(EventQueue& evq);
void buzzer_stop_alarm(EventQueue& evq);

static int id_gnss_window_tick  = 0;
static int id_gnss_window_start = 0;

static int id_mon_uplink   = 0;
static int id_alarm_uplink = 0;

static void cancel_task(EventQueue& evq, int& id) {
    if (id != 0) { evq.cancel(id); id = 0; }
}

const char* sm_state_str(SystemState s) {
    switch (s) {
        case STATE_LOCKED:     return "STATE_LOCKED";
        case STATE_MONITORING: return "STATE_MONITORING";
        case STATE_ALARM:      return "STATE_ALARM";
        default:               return "UNKNOWN";
    }
}

bool sm_is_locked(const SystemContext& ctx)     { return ctx.state == STATE_LOCKED; }
bool sm_is_monitoring(const SystemContext& ctx) { return ctx.state == STATE_MONITORING; }
bool sm_is_alarm(const SystemContext& ctx)      { return ctx.state == STATE_ALARM; }

void sm_init(SystemContext& ctx) {
    ctx.state = STATE_LOCKED;
}

static void gnss_window_tick(SystemContext* ctx) {
    if (!ctx) return;
    if (ctx->state != STATE_MONITORING) return;
    if (!ctx->gnss_window_active) return;

    if (ctx->gps_has_fix && ctx->stored_valid) {
        printf("[GNSS] FIX -> stored updated: %.6f, %.6f\r\n", ctx->stored_lat, ctx->stored_lon);
        gnss_stop_window(*ctx);
        return;
    }

    if (Kernel::Clock::now() >= ctx->gnss_window_deadline) {
        printf("[GNSS] window TIMEOUT (no fix)\r\n");
        gnss_stop_window(*ctx);
        return;
    }
}

void sm_set_state(SystemContext& ctx, EventQueue& evq, SystemState s) {
    if (ctx.state == s) return;

    SystemState old = ctx.state;
    ctx.state = s;

    printf("[STATE] %s -> %s\r\n", sm_state_str(old), sm_state_str(s));

    // stop periodic tasks from previous state
    cancel_task(evq, id_gnss_window_tick);
    cancel_task(evq, id_gnss_window_start);
    cancel_task(evq, id_mon_uplink);
    cancel_task(evq, id_alarm_uplink);

    buzzer_off();
    buzzer_stop_alarm(evq);

    if (s == STATE_LOCKED) {
        motion_set_enabled(ctx, false);

        if (ctx.gnss_window_active) ctx.gnss_window_active = false;

        // GNSS always sleep in LOCKED
        gnss_set_on(ctx, false);

    } else if (s == STATE_MONITORING) {
        motion_set_enabled(ctx, true);

        // GNSS only in window
        gnss_start_window(ctx);
        id_gnss_window_tick = evq.call_every(1s, callback(gnss_window_tick, &ctx));

        id_gnss_window_start = evq.call_every(GNSS_WINDOW_PERIOD, [&ctx]() {
            if (ctx.state == STATE_MONITORING && !ctx.gnss_window_active) {
                gnss_start_window(ctx);
            }
        });

        id_mon_uplink = evq.call_every(MONITOR_UPLINK_PERIOD, [&ctx]() {
            if (ctx.state == STATE_MONITORING) ctx.req_uplink_now = true;
        });

        ctx.req_uplink_now = true;

    } else if (s == STATE_ALARM) {
        motion_set_enabled(ctx, true);

        // GNSS always on in ALARM
        ctx.gnss_window_active = false;
        gnss_set_on(ctx, true);

        buzzer_start_alarm(evq);

        id_alarm_uplink = evq.call_every(ALARM_UPLINK_PERIOD, [&ctx]() {
            if (ctx.state == STATE_ALARM) ctx.req_uplink_now = true;
        });

        ctx.req_uplink_now = true;
    }
}
