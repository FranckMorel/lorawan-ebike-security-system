#pragma once
#include "mbed.h"
#include "state_machine.h"
#include <stdint.h>

struct Payload {
    uint8_t buf[13];
    uint8_t len = 13;
};

Payload payload_build(const SystemContext& ctx);
