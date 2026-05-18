#pragma once
#include "mbed.h"
#include "state_machine.h"
#include <cstdint>

bool rfid_init();
bool rfid_poll_and_handle(SystemContext& ctx, EventQueue& evq);
