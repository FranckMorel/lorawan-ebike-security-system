#pragma once
#include "mbed.h"
#include "state_machine.h"

bool gnss_init(SystemContext& ctx);
void gnss_uart_poll(SystemContext& ctx);

void gnss_set_on(SystemContext& ctx, bool on);

void gnss_start_window(SystemContext& ctx);
void gnss_stop_window(SystemContext& ctx);
