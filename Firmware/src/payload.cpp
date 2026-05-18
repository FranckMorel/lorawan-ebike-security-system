#include "payload.h"
#include <cmath>

Payload payload_build(const SystemContext& ctx) {
    Payload p{};

    uint8_t status = 0;
    if (ctx.state != STATE_LOCKED) status |= 0x01;
    if (ctx.state == STATE_ALARM)  status |= 0x02;
    if (ctx.gps_has_fix)           status |= 0x04; // FIX_NOW
    if (ctx.stored_valid)          status |= 0x08;
    status |= 0x10; // uses stored position

    p.buf[0] = status;

    float acc = ctx.last_accel_g;
    float acc100 = acc * 100.0f;
    if (acc100 < 0.0f) acc100 = 0.0f;
    if (acc100 > 255.0f) acc100 = 255.0f;

    p.buf[1] = (uint8_t)lroundf(acc100);

    int32_t lat_i = 0, lon_i = 0;
    if (ctx.stored_valid) {
        lat_i = (int32_t)lroundf(ctx.stored_lat * 1000000.0f);
        lon_i = (int32_t)lroundf(ctx.stored_lon * 1000000.0f);
    }

    p.buf[2] = (lat_i >> 24) & 0xFF;
    p.buf[3] = (lat_i >> 16) & 0xFF;
    p.buf[4] = (lat_i >>  8) & 0xFF;
    p.buf[5] = (lat_i      ) & 0xFF;

    p.buf[6] = (lon_i >> 24) & 0xFF;
    p.buf[7] = (lon_i >> 16) & 0xFF;
    p.buf[8] = (lon_i >>  8) & 0xFF;
    p.buf[9] = (lon_i      ) & 0xFF;

    p.buf[10] = (uint8_t)ctx.state;
    p.buf[11] = 0;
    p.buf[12] = 0;

    p.len = 13;
    return p;
}
