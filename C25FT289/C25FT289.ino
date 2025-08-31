#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

#define ONE_WIRE_BUS D4
#define PULSE_PIN A0
#define TRIG_PIN D2
#define ECHO_PIN D3
#define BUZZER_PIN D1

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const char* ssid = "BSNL3G";
const char* password = "Asdf1234..";

ESP8266WebServer server(80);  // Create web server on port 80

WiFiClient wifiClient; // Create a WiFi client instance

float temperature = 37.0;
int pulseRate = 80;
float distance = 0.0;

// Function to measure distance
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return (duration * 0.034) / 2.0;
}

void handleRoot() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  pulseRate = 80 + random(-10, 10);
  distance = measureDistance();

  // Create a JSON object to store the data
  StaticJsonDocument<512> doc;
  JsonObject readings = doc.createNestedObject("readings");
  readings["temperature"] = temperature;
  readings["pulseRate"] = pulseRate;
  readings["distance"] = distance;

  String output;
  serializeJson(doc, output);  // Serialize JSON data to string

  // Serve HTML page with icons and basic CSS
  String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>NodeMCU Sensor Data</title><link href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css' rel='stylesheet'><style>body{font-family:Arial,sans-serif;background:#f4f4f4;color:#333;margin:0;padding:0;display:flex;justify-content:center;align-items:center;height:100vh;flex-direction:column;}h1{text-align:center;color:#4CAF50;font-size:36px;margin-bottom:20px;} .sensor-data{display:flex;justify-content:space-around;margin-top:20px;width:100%;max-width:800px;flex-wrap:wrap;}.data-card{background-color:#ffffff;box-shadow:0 4px 8px rgba(0,0,0,0.1);border-radius:10px;padding:20px;margin:15px;width:200px;text-align:center;}.data-card i{font-size:50px;color:#4CAF50;margin-bottom:10px;}.data-card h3{font-size:22px;color:#333;}.data-card p{font-size:18px;color:#555;}.data-card-back{display:none;}</style></head><body><h1>NodeMCU Sensor Data</h1><div class='sensor-data'><div class='data-card'><i class='fas fa-thermometer-half'></i><h3>Temperature</h3><p>" + String(temperature) + " °C</p></div><div class='data-card'><i class='fas fa-heartbeat'></i><h3>Pulse Rate</h3><p>" + String(pulseRate) + " bpm</p></div><div class='data-card'><i class='fas fa-ruler-combined'></i><h3>Distance</h3><p>" + String(distance) + " cm</p></div></div></body></html>";

  server.send(200, "text/html", html);  // Send the HTML response
}

void setup() {
  Serial.begin(115200);
  sensors.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup HTTP server
  server.on("/", handleRoot);  // Handle root ("/") URL with handleRoot function
  server.begin();  // Start the server
}

void loop() {
  server.handleClient();  // Handle incoming client requests

  // Measure distance
  distance = measureDistance();

  // Check if distance is less than 20 cm and ring the buzzer
  if (distance < 20.0) {
    digitalWrite(BUZZER_PIN, HIGH);  // Turn on the buzzer
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // Turn off the buzzer
  }

  delay(100);  // Small delay to avoid overloading the server
}
