# STM32 LoRaWAN E-Bike Security System

Smart anti-theft and tracking system for E-Bikes based on STM32, LoRaWAN, GNSS, RFID authentication and MQTT-based monitoring.

---

## Overview

This project was developed as part of a practical embedded systems and IoT security application. The system is designed to detect unauthorized movement of an E-Bike, trigger alarm states and transmit tracking and status information over LoRaWAN.

The firmware was developed for STM32 ARM microcontrollers using C/C++ with a hardware-oriented and event-driven software architecture.

The system transmits telemetry data over LoRaWAN to The Things Network (TTN). The data is then forwarded via MQTT and visualized in OpenHAB for monitoring and status analysis.

---

## Features

* Motion detection using MPU sensor
* GNSS position tracking
* RFID user authentication
* LoRaWAN communication via The Things Network (TTN)
* MQTT for data transfer
* OpenHAB dashboard visualization
* Event-driven alarm state machine
* Periodic uplink transmission
* System status monitoring
* Power management


---

## Hardware Components

* STM32L072CZ-LRWAN1 Discovery Board
* GNSS module
* MPU6050 accelerometer
* MFRC522 RFID reader
* LoRa transceiver


---

## Software Architecture

The firmware is structured into modular embedded software components:

* Sensor drivers
* RFID authentication
* GNSS handling
* LoRaWAN communication
* Alarm state machine
* Event handling


---

## Technologies

* C / C++
* LoRaWAN
* MQTT
* OpenHAB
* GNSS
* RFID
* SPI / UART / I2C


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

* Mbed Studio / MbedOS
* Logic Analyzer
* Oscilloscope
* ST-Link Debugger

---

## Future Improvements

* Mobile application integration
* BLE integration for local communication and configuration
* Secure firmware update support

---

## Author

Franck Morel Tonfack

LinkedIn: https://linkedin.com/in/morel-tonfack98
