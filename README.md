# STM32 LoRaWAN E-Bike Security System

Embedded anti-theft and tracking system for E-Bikes based on STM32, LoRaWAN, GNSS, RFID authentication and MQTT-based monitoring.

---

## Overview

This project was developed as part of a practical embedded systems and IoT security application. The system is designed to detect unauthorized movement of an E-Bike, trigger alarm states and transmit tracking and status information over LoRaWAN.

The firmware was developed for STM32 ARM microcontrollers using C/C++ with a hardware-oriented and event-driven software architecture.

In addition to the embedded firmware, the project integrates MQTT-based data forwarding and visualization using OpenHAB for remote monitoring and system interaction.

---

## Features

* Motion detection using MPU sensor
* GNSS position tracking
* RFID-based user authentication
* LoRaWAN communication via The Things Network (TTN)
* MQTT data forwarding
* OpenHAB dashboard visualization
* Event-driven alarm state machine
* Low power operation
* Periodic uplink transmission
* System status monitoring

---

## Hardware Components

* STM32L072CZ-LRWAN1 Discovery Board
* GNSS module
* MPU6050 accelerometer
* MFRC522 RFID reader
* LoRa transceiver
* Battery-powered embedded system

---

## Software Architecture

The firmware is structured into modular embedded software components:

* Sensor drivers
* RFID authentication
* GNSS handling
* LoRaWAN communication
* MQTT integration
* Alarm state machine
* Power management
* Embedded event handling

The system transmits telemetry data over LoRaWAN to The Things Network (TTN). The data is then forwarded via MQTT and visualized in OpenHAB for monitoring and status analysis.

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

* Mbed Studio
* Git / GitHub
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
