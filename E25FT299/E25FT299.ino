#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi Credentials
const char* ssid = "YourSSID";
const char* password = "YourPassword";

// Pins
#define MQ_SENSOR_PIN A0
#define DHT_PIN D2
#define ONE_WIRE_BUS D3

// DHT Sensor
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// DS18B20 Sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Web server
ESP8266WebServer server(80);

// Variables
float mqGasValue = 0.0;
float dhtTemperature = 0.0, dhtHumidity = 0.0;
float ds18b20Temperature = 0.0;

// Setup
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  // Display the IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  dht.begin();
  sensors.begin();

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

// Loop
void loop() {
  server.handleClient();
  readSensors();
}

// Read sensor values
void readSensors() {
  mqGasValue = analogRead(MQ_SENSOR_PIN);
  dhtTemperature = dht.readTemperature();
  dhtHumidity = dht.readHumidity();

  sensors.requestTemperatures();
  ds18b20Temperature = sensors.getTempCByIndex(0);
}

// Handle root
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head>";
  html += "<title>Sensor Data</title>";
  html += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css'>";
  html += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css'>";
  html += "<style>";
  html += ".card { border-radius: 15px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); transition: 0.3s; }";
  html += ".card:hover { transform: scale(1.05); box-shadow: 0 8px 16px rgba(0, 0, 0, 0.3); }";
  html += ".icon { font-size: 2rem; color: #007bff; }";
  html += "</style>";
  html += "</head>";
  html += "<body class='bg-light'>";
  html += "<div class='container mt-5'>";
  html += "<h1 class='text-center mb-4'>Sensor Readings</h1>";
  html += "<div class='row'>";
  
  html += "<div class='col-md-4'>";
  html += "<div class='card text-center p-3'>";
  html += "<i class='fas fa-gas-pump icon'></i>";
  html += "<h5 class='mt-3'>MQ Gas Sensor</h5>";
  html += "<p class='mb-0'>" + String(mqGasValue) + "</p>";
  html += "</div>";
  html += "</div>";

  html += "<div class='col-md-4'>";
  html += "<div class='card text-center p-3'>";
  html += "<i class='fas fa-thermometer-half icon'></i>";
  html += "<h5 class='mt-3'>DHT11 Temperature</h5>";
  html += "<p class='mb-0'>" + String(dhtTemperature) + " °C</p>";
  html += "</div>";
  html += "</div>";

  html += "<div class='col-md-4'>";
  html += "<div class='card text-center p-3'>";
  html += "<i class='fas fa-water icon'></i>";
  html += "<h5 class='mt-3'>DHT11 Humidity</h5>";
  html += "<p class='mb-0'>" + String(dhtHumidity) + " %</p>";
  html += "</div>";
  html += "</div>";

  html += "<div class='col-md-4'>";
  html += "<div class='card text-center p-3'>";
  html += "<i class='fas fa-temperature-low icon'></i>";
  html += "<h5 class='mt-3'>DS18B20 Temperature</h5>";
  html += "<p class='mb-0'>" + String(ds18b20Temperature) + " °C</p>";
  html += "</div>";
  html += "</div>";

  html += "</div>"; // row
  html += "</div>"; // container
  html += "</body></html>";

  server.send(200, "text/html", html);
}
