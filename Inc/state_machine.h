#pragma once
#include "mbed.h"
#include <cstdint>
#include <stdint.h>

enum SystemState : uint8_t {
    STATE_LOCKED     = 0,
    STATE_MONITORING = 1,
    STATE_ALARM      = 2
};

struct SystemContext {
    volatile SystemState state = STATE_LOCKED;

    // Sensor / runtime
    bool  mpu_enabled = false;
    float last_accel_g = 0.0f;

    bool  gnss_on = false;
    bool  gnss_uart_poll_enabled = false;

    bool  gps_has_fix = false;   // FIX_NOW
    float gps_latitude = 0.0f;
    float gps_longitude = 0.0f;

    bool  stored_valid = false;
    float stored_lat = 0.0f;
    float stored_lon = 0.0f;

    bool gnss_window_active = false;
    Kernel::Clock::time_point gnss_window_deadline;

    // Uplink triggers
    volatile bool req_uplink_now   = false;
    volatile bool req_first_uplink = false;
    volatile bool req_retry_uplink = false;
};

const char* sm_state_str(SystemState s);

void sm_init(SystemContext& ctx);
void sm_set_state(SystemContext& ctx, EventQueue& evq, SystemState s);

// Used by other modules
bool sm_is_locked(const SystemContext& ctx);
bool sm_is_monitoring(const SystemContext& ctx);
bool sm_is_alarm(const SystemContext& ctx);
