# 🌱 Smart Irrigation System (NodeMCU ESP8266)

Smart, IoT-enabled irrigation prototype using a **DHT11** (temperature & humidity), **soil moisture sensor**, **OLED (SPI)** display, and a **relay** to control a water pump. Sensor data is displayed locally on an OLED and optionally pushed to **ThingSpeak** for remote logging.

---

## 🧠 Project Summary

This project automates irrigation by reading soil moisture and environmental conditions, switching a pump ON/OFF through a relay (active LOW), and displaying live values on an SPI OLED. It also uploads telemetry to ThingSpeak for monitoring and logging.

**Key goals:** reduce water usage, enable remote monitoring, and demonstrate a low-cost, easily-replicable IoT solution.

---

## ⚙️ Features

- Real-time soil moisture measurement (calibrated to percentage)
- Temperature & humidity read via DHT11
- Local SPI OLED display (128×64) showing soil RH, humidity, temperature, and pump state
- Automatic pump control using an active-LOW relay based on moisture threshold
- Data logging to ThingSpeak (API key)
- Simple adjustable calibration constants for dry/wet sensor readings
- Easy-to-adapt code for different microcontrollers / sensors

---

## 🧩 Hardware Components

- NodeMCU ESP8266 (or equivalent ESP8266 board)  
- DHT11 sensor (temperature & humidity)  
- Soil moisture analog sensor (voltage output -> A0)  
- OLED display (SSD1306, SPI interface)  
- 1-channel Relay module (active LOW recommended)  
- Jumper wires, breadboard, power supply, water pump (use appropriate power / isolation)

---

## 🧭 Pin Mapping (as used in code)

> NodeMCU pin names (Dx) are used in the code. Adjust if you use raw GPIO numbers.

- **OLED (SPI)**  
  - MOSI -> `D3`  
  - SCK  -> `D4`  
  - DC   -> `D8`  
  - CS   -> `D6`  
  - RESET-> not connected (`-1`)  

- **DHT11** -> `D2`  
- **Soil sensor (analog)** -> `A0`  
- **Relay** -> `D1` (code uses `#define relayPin 5` which corresponds to D1 — check board mapping)  
  - *Note:* In the provided code `#define relayPin 5`. On NodeMCU, GPIO5 corresponds to `D1`. Use GPIO mapping appropriate to your board.

> If using Arduino IDE pin labels, match NodeMCU Dn to GPIO numbers (or update code to use `D1`, `D2`, etc. directly if supported).

---

## 🔧 Software & Libraries

- Arduino IDE (or PlatformIO)
- Libraries required:
  - `ESP8266WiFi`
  - `DHT` (Adafruit or equivalent)
  - `Adafruit_GFX`
  - `Adafruit_SSD1306`
  - `SPI` (built-in)
- ThingSpeak account + Write API Key (optional for remote logging)

---

## 📥 How to Use

1. Clone the repo:
   ```bash
   git clone https://github.com/Ritesh1coder/smart-irrigation.git
   cd smart-irrigation
