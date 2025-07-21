# 🏠 Smart Home Dashboard with ESP8266

A real-time web-based smart home monitoring and control system using ESP8266, DHT22 (temperature and humidity), MQ135 (air quality), and PWM-controlled fan and LED.

## 🚀 Features

- 🌡️ Live temperature and humidity readings via **DHT22**
- 🌫️ Air quality monitoring using **MQ135 (CO₂ ppm)**
- 🌀 Fan speed control via PWM
- 💡 LED brightness control via PWM
- 📡 Real-time data updates via **WebSocket**
- 🌐 Accessible via `http://esp.local` using **mDNS**

---

## 📷 Web Dashboard

Built with plain HTML, CSS & JavaScript:
- Display of temperature, humidity, and CO₂ data
- Fan speed and LED brightness sliders
- Status of WebSocket connection

---

## 🔧 Hardware Connections

| Component | Pin         | Description               |
|----------|-------------|---------------------------|
| DHT22     | GPIO2 (D4)  | Temperature & Humidity    |
| MQ135     | A0          | CO₂ sensing (analog input)|
| Fan       | GPIO12 (D6) | PWM controlled            |
| LED       | GPIO13 (D7) | PWM controlled            |

---

## 📦 Libraries Used

- `ESP8266WiFi.h` – Wi-Fi connectivity
- `WebSocketsServer.h` – WebSocket server
- `ESP8266WebServer.h` – HTTP server
- `ESP8266mDNS.h` – mDNS for `esp.local`
- `DHT.h` – DHT sensor handling
- `MQ135.h` – Air quality sensor
- `Hash.h` – Required by WebSocket library

---

## 🔌 Wi-Fi Configuration

Edit your Wi-Fi credentials in the code:

