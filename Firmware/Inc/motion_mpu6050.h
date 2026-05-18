#pragma once
#include "mbed.h"

class MotionMPU6050 {
public:
    struct Sample {
        bool ok;
        float mag_g;
        float delta_g;
    };

    MotionMPU6050(PinName sda, PinName scl, uint8_t addr7 = 0x68);

    bool init(uint32_t freq = 400000);
    void set_enabled(bool on);
    bool is_enabled() const { return _enabled; }

    bool poll(Sample &out);
    bool motion_detected(float threshold_delta_g);

private:
    bool write_reg(uint8_t reg, uint8_t val);
    bool read_regs(uint8_t reg, uint8_t *buf, int len);

private:
    I2C _i2c;
    int _addr8;
    bool _enabled = false;
    bool _present = false;

    float _ema = 1.0f;
    bool _ema_init = false;
};
