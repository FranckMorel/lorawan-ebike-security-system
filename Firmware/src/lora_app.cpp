#include "lora_app.h"
#include <cstdio>

LoraApp::LoraApp(LoRaWANInterface &lw, EventQueue &q)
    : _lw(lw), _q(q) {}

bool LoraApp::init() {
    if (_lw.initialize(&_q) != LORAWAN_STATUS_OK) {
        printf("[LoRa] init fail\r\n");
        return false;
    }

    _cbs.events = mbed::callback(this, &LoraApp::on_event);
    _lw.add_app_callbacks(&_cbs);

    _lw.enable_adaptive_datarate();
    _lw.set_confirmed_msg_retries(0);

    printf("[LoRa] init OK\r\n");
    return true;
}

bool LoraApp::connect() {
    printf("[LoRa] connecting...\r\n");
    auto st = _lw.connect();
    printf("[LoRa] connect status=%d\r\n", st);
    return true;
}

bool LoraApp::send(uint8_t port, const uint8_t *buf, uint8_t len) {
    if (!_connected) {
        printf("[LoRa] not connected\r\n");
        return false;
    }

    if (_tx_busy) return false;

    _tx_busy = true;
    int ret = _lw.send(port, buf, len, MSG_UNCONFIRMED_FLAG);

    if (ret < 0) {
        printf("[LoRa] send error %d\r\n", ret);
        _tx_busy = false;
        return false;
    }

    printf("[LoRa] TX queued (%d bytes)\r\n", ret);
    return true;
}

void LoraApp::on_event(lorawan_event_t ev) {
    printf("[LoRa] EVENT=%d\r\n", ev);

    switch(ev) {
        case CONNECTED:
            _connected = true;
            printf("[LoRa] CONNECTED\r\n");
            break;

        case TX_DONE:
            _tx_busy = false;
            printf("[LoRa] TX_DONE\r\n");
            break;

        case TX_ERROR:
        case TX_TIMEOUT:
            _tx_busy = false;
            printf("[LoRa] TX_FAIL\r\n");
            break;

        default:
            break;
    }
}
