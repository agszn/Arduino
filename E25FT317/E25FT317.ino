#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>

// WiFi Credentials
const char* ssid = "BSNL3G";
const char* password = "Asdf1234..";

// Django Server URL
const char* serverUrl = "http://127.0.0.1:8000/update_sensor_data/";


WiFiClient client;
HTTPClient http;

// Pin Definitions
#define PULSE_PIN A0
#define DHT_PIN D4
#define DS18B20_PIN D3
#define DHT_TYPE DHT11

// Sensor Objects
DHT dht(DHT_PIN, DHT_TYPE);
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  dht.begin();
  sensors.begin();
  if (!accel.begin()) {
    Serial.println("Failed to initialize ADXL345!");
    while (1);
  }
}

void loop() {
  // Read Sensor Data
  int pulseValue = analogRead(PULSE_PIN);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  sensors.requestTemperatures();
  float ds18b20Temp = sensors.getTempCByIndex(0);
  sensors_event_t event;
  accel.getEvent(&event);
  
  // Prepare Data JSON
  String payload = "{\"pulse\":" + String(pulseValue) +
                   ",\"temperature\":" + String(temperature) +
                   ",\"humidity\":" + String(humidity) +
                   ",\"ds18b20\":" + String(ds18b20Temp) +
                   ",\"accel_x\":" + String(event.acceleration.x) +
                   ",\"accel_y\":" + String(event.acceleration.y) +
                   ",\"accel_z\":" + String(event.acceleration.z) + "}";

  Serial.println(payload);

  // Send Data to Django Server
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(client, serverUrl);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.println("Data Sent Successfully");
    } else {
      Serial.print("Error Sending Data: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }

  delay(5000);  // Send data every 5 seconds
}
