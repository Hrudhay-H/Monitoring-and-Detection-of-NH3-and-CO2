# Monitoring-and-Detection-of-NH3-and-CO2
# PURAIR 🐔🌱 — Smart Air Quality Monitoring for Poultry Farms

**PURAIR** is an AI + IoT-based smart air quality monitoring system designed specifically for poultry farms to ensure healthier environments for birds and improved egg production. It continuously tracks levels of **ammonia (NH₃)**, **carbon dioxide (CO₂)**, and **humidity**, then displays and sends real-time data to both a web server and external dashboards.

> 🏅 Presented at **The Design & Innovation Clinic - 2025** and evaluated by **CMTI**  
> 💡 Built using ESP32, sensors (MQ series & DHT11), Web Server, and Python (Flask)

---

## 🔧 Features

- 🌐 **Real-time Wi-Fi connectivity** using ESP32
- 🧪 **Gas level detection**:
  - NH₃ (Ammonia) sensor
  - CO₂ (Carbon Dioxide) sensor
  - DHT11 for humidity sensing
- 🚨 **Buzzer alert system** when gas levels exceed safe thresholds
- 📊 Sends data to a Flask-based server for visualization or analytics
- 📡 Built-in web server (ESP32) with endpoints:
  - `/nh3`
  - `/co2`
  - `/humidity`

---

## 🛠️ Hardware Requirements

- ESP32 Dev Module
- MQ-135 (or MQ-137 for NH₃)
- MQ-9 (or MQ-2 for CO₂)
- DHT11 sensor
- Piezo Buzzer
- Jumper Wires, Breadboard

---

## 💻 Software Requirements

- Arduino IDE (with ESP32 board support)
- Python 3.x (for Flask server)
- Required Libraries:
  - `WiFi.h`
  - `WebServer.h`
  - `HTTPClient.h`
  - `DHT.h` (DHT sensor library)

---

## 📁 Repository Structure

PURAIR/ ├── poultry_monitor.ino # Main Arduino sketch ├── flask_server/ # Flask-based server to receive data │ └── app.py ├── README.md └── images/ # Circuit diagrams or setup images

yaml
Copy
Edit

---

## 🚀 Getting Started

### Arduino (ESP32)

1. Install required libraries via Library Manager (`WiFi`, `WebServer`, `DHT`)
2. Set your Wi-Fi credentials in the sketch
3. Flash the code to ESP32
4. Access the ESP32's IP in a browser to view JSON data


### 📌 Future Improvements
1. Integration with Blynk/ThingSpeak for remote dashboards
2. SMS/Email alerts for farm managers
3. Solar-powered IoT module
4. ML model to forecast gas spikes

### Flask Server

```bash
cd flask_server
pip install flask
python app.py
Your ESP32 will push sensor readings to this server at:

arduino
Copy
Edit
http://<YOUR-IP>:5000/sensor
📡 API Endpoints (ESP32 Web Server)
Endpoint	Method	Description
/	GET	Root - optional HTML page
/nh3	GET	NH₃ sensor reading (JSON)
/co2	GET	CO₂ sensor reading (JSON)
/humidity	GET	Humidity reading (JSON)
📈 Sample JSON Response
json
Copy
Edit
{
  "gas": "CO2",
  "value": 850,
  "status": "High",
  "note": "⚠ May affect egg production!"
}
```

Support from PESCE & CMTI Evaluators

### 📜 License
MIT License — free to use and modify for educational or commercial purposes. Give credit where it's due 😊

