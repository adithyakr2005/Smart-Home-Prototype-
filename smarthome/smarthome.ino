#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DHT.h>
#include <MQ135.h>
#include <Hash.h>

// 🔌 Wi-Fi Credentials
const char* ssid = "Om gal";
const char* password = "om123456789";

// 🌡️ Sensor Pins & Setup
#define DHTPIN D4          // GPIO2
#define DHTTYPE DHT22
#define MQ135PIN A0        // Analog input

DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135_sensor(MQ135PIN);

// 🌀 Fan control
#define FAN_PIN D6         // PWM pin for fan
#define LED_PIN D7 

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Calibration variable for MQ135
float R0 = 10.0;

void setup() {
  Serial.begin(115200);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\n✅ Wi-Fi Connected!");
  Serial.print("📡 IP: "); Serial.println(WiFi.localIP());

  // Start mDNS
  if (MDNS.begin("esp")) {
    Serial.println("🌐 mDNS started: http://esp.local");
  } else {
    Serial.println("❌ mDNS failed");
  }

  // Calibrate MQ135 in clean air
  Serial.println("🔍 Calibrating MQ135...");
  R0 = mq135_sensor.getRZero();
  Serial.print("✅ R0: "); Serial.println(R0);

  // Start server and WebSocket
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("🚀 WebSocket server started");
}

void loop() {
  webSocket.loop();
  server.handleClient();
  MDNS.update();

  static unsigned long lastSent = 0;
  if (millis() - lastSent >= 5000) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    float ppm = mq135_sensor.getPPM();

    // Sensor check
    if (!isnan(temperature) && !isnan(humidity)) {
      String json = "{";
      json += "\"temp\":" + String(temperature, 2) + ",";
      json += "\"humidity\":" + String(humidity, 2) + ",";
      json += "\"co2_ppm\":" + String(ppm, 2);
      json += "}";

      Serial.println("📤 Sending: " + json);
      webSocket.broadcastTXT(json);
    } else {
      Serial.println("❌ Error reading DHT22");
    }

    lastSent = millis();
  }
}

// 🔁 WebSocket Events
void webSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    Serial.print("📨 Received: "); Serial.println(msg);

    if (msg.startsWith("fan:")) {
      int speed = msg.substring(4).toInt();  // fan:80
      int pwmf = map(speed, 0, 100, 0, 1023);
      analogWrite(FAN_PIN, pwmf);
      Serial.print("🌀 Fan set to "); Serial.print(speed); Serial.println("%");
    }
    if (msg.startsWith("led:")) {
      int brightness = msg.substring(4).toInt();  // led:75
      int pwml = map(brightness, 0, 100, 0, 1023);
      analogWrite(LED_PIN, pwml);
      Serial.print("💡 LED brightness set to "); Serial.print(brightness); Serial.println("%");
    }
  }
}
