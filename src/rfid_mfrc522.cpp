#include "rfid_mfrc522.h"
#include "app_config.h"
#include "MFRC522.h"
#include <cstring>
#include <stdint.h>

// Master UID
static const uint8_t MASTER_UID[] = { 0xCC, 0x50, 0x86, 0x04 };
static const uint8_t MASTER_UID_LEN = sizeof(MASTER_UID);

static MFRC522 rfid(RFID_MOSI, RFID_MISO, RFID_SCK, RFID_CS, RFID_RST);

void buzzer_beep_ms(EventQueue& evq, std::chrono::milliseconds ms_on);

static bool rfid_read_uid(uint8_t* uid, uint8_t& uid_len) {
    if (!rfid.PICC_IsNewCardPresent()) return false;
    if (!rfid.PICC_ReadCardSerial())   return false;

    uid_len = rfid.uid.size;
    if (uid_len > 10) uid_len = 10;

    for (uint8_t i = 0; i < uid_len; i++) uid[i] = rfid.uid.uidByte[i];

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    return true;
}

bool rfid_init() {
    rfid.PCD_Init();
    printf("[RFID] init OK\r\n");
    return true;
}

bool rfid_poll_and_handle(SystemContext& ctx, EventQueue& evq) {
    uint8_t uid[10];
    uint8_t uid_len = 0;

    if (!rfid_read_uid(uid, uid_len)) return false;

    printf("[RFID] UID: ");
    for (uint8_t i = 0; i < uid_len; i++) printf("%02X", uid[i]);
    printf("\r\n");

    bool is_master = (uid_len == MASTER_UID_LEN) && (memcmp(uid, MASTER_UID, MASTER_UID_LEN) == 0);
    if (!is_master) return true;

    buzzer_beep_ms(evq, 80ms);

    // State toggling logic:
    if (ctx.state == STATE_LOCKED) ctx.state = STATE_MONITORING;
    else if (ctx.state == STATE_MONITORING) ctx.state = STATE_LOCKED;
    else if (ctx.state == STATE_ALARM) ctx.state = STATE_MONITORING;

    return true;
}
