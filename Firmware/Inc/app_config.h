#pragma once
#include "mbed.h"
#include <cstdint>
#include <chrono>

using namespace std::chrono_literals;

// ==============================
// Compile-time config
// ==============================
static constexpr bool GNSS_HAS_HW_SLEEP = true;   // 10k nach GND vorhanden -> WAKE=0 ist definiert
static constexpr float TH_HIGH_G = 1.20f;         // Alarm-Schwelle in g (Betrag)

// ==============================
// Periods
// ==============================
static constexpr auto RFID_POLL_PERIOD      = 100ms;
static constexpr auto MOTION_POLL_PERIOD    = 250ms;
static constexpr auto GNSS_POLL_PERIOD      = 100ms;
static constexpr auto STATUS_PRINT_PERIOD   = 2s;

static constexpr auto MONITOR_UPLINK_PERIOD = 60s;
static constexpr auto GNSS_WINDOW_PERIOD    = 60s;
static constexpr auto GNSS_WINDOW_LENGTH    = 300s;

static constexpr auto ALARM_UPLINK_PERIOD   = 20s;
static constexpr auto ALARM_BEEP_PERIOD     = 2s;

// GNSS timings
static constexpr auto GNSS_WAKE_SETTLE_TIME  = 250ms;
static constexpr auto GNSS_SLEEP_SETTLE_TIME = 150ms;
static constexpr auto GNSS_WAKE_KICK_WAIT    = 50ms;
static constexpr auto GNSS_STANDBY_CMD_WAIT  = 120ms;

// ==============================
// Pins (DISCO-L072CZ-LRWAN1)
// ==============================
// RFID MFRC522 (SPI)
static constexpr PinName RFID_MOSI = D11;
static constexpr PinName RFID_MISO = D12;
static constexpr PinName RFID_SCK  = D13;
static constexpr PinName RFID_CS   = D10;
static constexpr PinName RFID_RST  = D9;

// Buzzer
static constexpr PinName BUZZER_PIN = D7;

// MPU6050 (I2C)
static constexpr PinName I2C_SDA = D14;
static constexpr PinName I2C_SCL = D15;
static constexpr int MPU6050_ADDR_7BIT = 0x68; 

// GNSS (PA1010D)
static constexpr PinName GNSS_TX = PA_9;   // MCU RX (GNSS TX -> MCU RX)
static constexpr PinName GNSS_RX = PA_10;  // MCU TX (GNSS RX -> MCU TX)
static constexpr int GNSS_BAUD = 9600;

static constexpr PinName GNSS_WAKE_PIN = PA_4;  // 0 = sleep
