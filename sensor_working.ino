#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "DHT.h"

// WiFi credentials
const char* ssid = "vivo";
const char* password = "Arun1234";

// Manual server URL
const char* serverName = "http://192.168.210.72:5000/sensor";

WebServer server(80);

// Sensor pins
const int nh3Pin = 34; // Analog pin for NH₃ sensor
const int co2Pin = 35; // Analog pin for CO₂ sensor
const int buzzer = 33;


#define DHTPIN 32       // Digital pin for DHT sensor
#define DHTTYPE DHT11   // DHT11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

// Threshold values
const int nh3Threshold = 1000;
const int co2Threshold = 500;


String getGasStatus(int value, int threshold, const String& gas) {
  String status = (value > threshold) ? "High" : "Low";
  String warning = (value > threshold) ? "⚠ May affect egg production!" : "✅ Safe level";
  return "{\"gas\":\"" + gas + "\",\"value\":" + value + ",\"status\":\"" + status + "\",\"note\":\"" + warning + "\"}";
}



  server.send(200, "text/html", html);
}

void handleNH3() {
  int value = analogRead(nh3Pin);
  server.send(200, "application/json", getGasStatus(value, nh3Threshold, "NH3"));
}

void handleCO2() {
  int value = analogRead(co2Pin);
  server.send(200, "application/json", getGasStatus(value, co2Threshold, "CO2"));
}

void handleHumidity() {
  float h = dht.readHumidity();

  if (isnan(h)) {
    server.send(500, "application/json", "{\"error\":\"Failed to read humidity\"}");
    return;
  }

  //Given an example threshold 
  String status = (h < 30 || h > 70) ? "High/Low" : "Normal";
  String note = (h < 30 || h > 70) ? "⚠ May affect poultry health!" : "✅ Optimal humidity";

  String json = "{\"gas\":\"Humidity\",\"value\":" + String(h, 1) + ",\"status\":\"" + status + "\",\"note\":\"" + note + "\"}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n🔌 Starting ESP32...");
  WiFi.begin(ssid, password);

  Serial.print("📡 Connecting to Wi-Fi");
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    retry++;
    if (retry > 20) {
      Serial.println("\n❌ Failed to connect to Wi-Fi. Check SSID or password.");
      return;
    }
  }

  Serial.println("\n✅ Connected to Wi-Fi!");
  Serial.print("🌐 Web Server IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();

  pinMode(buzzer, OUTPUT);
  

  server.on("/", handleRoot);
  server.on("/nh3", handleNH3);
  server.on("/co2", handleCO2);
  server.on("/humidity", handleHumidity);
  server.begin();
  Serial.println("🚀 Web server started...");
}

void loop() {
  server.handleClient();

  int nh3Value = analogRead(nh3Pin);
  int co2Value = analogRead(co2Pin);

if (nh3Value > nh3Threshold || co2Value > co2Threshold) {
    Serial.println("⚠ Gas levels high! Buzzer ON");
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }

  // Send data to external/manual server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"nh3\": " + String(nh3Value) + ", \"co2\": " + String(co2Value) + "}";
    int httpResponseCode = http.POST(json);

    
    http.end();
  }
}
