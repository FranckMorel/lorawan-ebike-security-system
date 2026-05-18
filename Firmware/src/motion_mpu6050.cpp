#include "motion_mpu6050.h"
#include <cmath>
#include <cstdio>

static constexpr uint8_t REG_PWR = 0x6B;
static constexpr uint8_t REG_ACCEL = 0x3B;
static constexpr uint8_t REG_WHOAMI = 0x75;

MotionMPU6050::MotionMPU6050(PinName sda, PinName scl, uint8_t addr7)
    : _i2c(sda, scl), _addr8(addr7 << 1) {}

bool MotionMPU6050::write_reg(uint8_t reg, uint8_t val) {
    char d[2] = {(char)reg, (char)val};
    return _i2c.write(_addr8, d, 2) == 0;
}

bool MotionMPU6050::read_regs(uint8_t reg, uint8_t *buf, int len) {
    char r = reg;
    if (_i2c.write(_addr8, &r, 1, true) != 0) return false;
    if (_i2c.read(_addr8, (char*)buf, len) != 0) return false;
    return true;
}

bool MotionMPU6050::init(uint32_t freq) {
    _i2c.frequency(freq);

    uint8_t who = 0;
    if (!read_regs(REG_WHOAMI, &who, 1)) {
        printf("[MPU] not found\r\n");
        _present = false;
        return false;
    }

    printf("[MPU] WHO_AM_I=0x%02X\r\n", who);
    _present = true;

    write_reg(REG_PWR, 0x40); // sleep
    _enabled = false;

    return true;
}

void MotionMPU6050::set_enabled(bool on) {
    if (!_present) return;

    if (on) {
        write_reg(REG_PWR, 0x00);
        printf("[MPU] enabled\r\n");
    } else {
        write_reg(REG_PWR, 0x40);
        printf("[MPU] sleep\r\n");
    }
    _enabled = on;
}

bool MotionMPU6050::poll(Sample &out) {
    out.ok = false;
    if (!_enabled || !_present) return false;

    uint8_t buf[6];
    if (!read_regs(REG_ACCEL, buf, 6)) return false;

    int16_t ax = (buf[0]<<8)|buf[1];
    int16_t ay = (buf[2]<<8)|buf[3];
    int16_t az = (buf[4]<<8)|buf[5];

    float scale = 1.0f/16384.0f;
    float fx = ax*scale;
    float fy = ay*scale;
    float fz = az*scale;

    float mag = sqrtf(fx*fx + fy*fy + fz*fz);

    if (!_ema_init) {
        _ema = mag;
        _ema_init = true;
    } else {
        _ema = 0.8f*_ema + 0.2f*mag;
    }

    out.mag_g = _ema;
    out.delta_g = fabsf(_ema - 1.0f);
    out.ok = true;
    return true;
}

bool MotionMPU6050::motion_detected(float threshold) {
    Sample s;
    if (!poll(s)) return false;
    return s.delta_g > threshold;
}
