# STM32 LoRaWAN E-Bike Security System

Smart anti-theft and tracking system for E-Bikes based on STM32, LoRaWAN, GNSS, RFID authentication and MQTT-based monitoring.

---

## Overview

This project implements a smart embedded anti-theft and tracking system for E-Bikes using STM32 and LoRaWAN technology.

The system detects unauthorized movement, authenticates users via RFID, acquires GNSS location data and transmits telemetry over LoRaWAN to The Things Network (TTN).

The firmware is written in C/C++ using a modular and hardware-oriented embedded software architecture.

---

## Features

- Motion and vibration detection using MPU6050
- GNSS position tracking
- RFID-based user authentication
- LoRaWAN communication via TTN
- Event-driven alarm state machine
- Periodic uplink transmission
- Power management
- Modular driver-based firmware architecture

---

## Hardware Components

| Component | Description |
|---|---|
| STM32L072CZ-LRWAN1 | Main microcontroller and LoRa platform |
| MPU6050 | Motion and vibration sensor |
| MFRC522 | RFID authentication reader |
| GNSS Module | Position tracking |
| SX1276 LoRa Transceiver | LoRaWAN communication |

---

## Wiring

### GNSS Module (UART)

| GNSS Pin | STM32 Pin |
|---|---|
| TX | PA10 (USART1_RX) |
| RX | PA9 (USART1_TX) |
| VCC | 3.3V |
| GND | GND |

---

### MPU6050 (I2C)

| MPU6050 Pin | STM32 Pin |
|---|---|
| SDA | PB9 (I2C1_SDA) |
| SCL | PB8 (I2C1_SCL) |
| VCC | 3.3V |
| GND | GND |

---

### MFRC522 RFID Reader (SPI)

| MFRC522 Pin | STM32 Pin |
|---|---|
| SDA / NSS | PA4 (SPI1_NSS) |
| SCK | PA5 (SPI1_SCK) |
| MISO | PA6 (SPI1_MISO) |
| MOSI | PA7 (SPI1_MOSI) |
| RST | GPIO |
| 3.3V | 3.3V |
| GND | GND |

LoRa Transceiver

The SX1276 LoRa transceiver is integrated on the STM32L072CZ-LRWAN1 Discovery Board and internally connected to the MCU SPI interface.

---

## Firmware Modules

```text
main.cpp              -> Main application entry point
state_machine.cpp     -> Alarm and system state handling
motion_mpu6050.cpp    -> MPU6050 motion sensor driver
rfid_mfrc522.cpp      -> RFID authentication driver
gnss_pa1010d.cpp      -> GNSS handling and parsing
lora_app.cpp          -> LoRaWAN communication
payload.cpp           -> Payload encoding and formatting
```

---

## Project Structure

```text
Firmware/      -> Embedded firmware source code
Hardware/      -> Schematics and PCB files
Docs/          -> Technical documentation
Media/         -> Images and project screenshots
```

---

## Development Tools

- Mbed Studio
- MbedOS
- STM32CubeMX
- Logic Analyzer
- Oscilloscope
- ST-Link Debugger

---

## Future Improvements

- BLE integration for local communication
- Mobile application support
- OTA firmware update support

---

## Electrical Design Notes

- All peripherals operate at 3.3V logic levels
- Shared GND between all modules is required
- I2C lines require pull-up resistors
- SPI traces should remain short for signal integrity
- Decoupling capacitors are recommended near each peripheral

---

## LoRaWAN Stack

The LoRaWAN stack integration is based on the official MbedOS LoRaWAN example application provided by Arm Mbed.

The project extends and adapts the original example implementation with custom application logic, payload handling, sensor integration and state machine for the E-Bike security use case.

## Author

**Franck Morel Tonfack**

LinkedIn: https://linkedin.com/in/morel-tonfack98
