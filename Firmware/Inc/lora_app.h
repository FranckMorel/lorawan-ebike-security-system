#pragma once
#include "mbed.h"
#include "LoRaWANInterface.h"
#include "events/EventQueue.h"

class LoraApp {
public:
    LoraApp(LoRaWANInterface &lw, EventQueue &q);

    bool init();
    bool connect();

    bool send(uint8_t port, const uint8_t *buf, uint8_t len);

    bool is_connected() const { return _connected; }

private:
    void on_event(lorawan_event_t ev);

private:
    LoRaWANInterface &_lw;
    EventQueue &_q;
    lorawan_app_callbacks_t _cbs;

    volatile bool _connected = false;
    volatile bool _tx_busy = false;
};
