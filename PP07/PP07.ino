#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>

// Wi-Fi Network Credentials (for Access Point mode)
const char* ssid = "ESP8266_AP";  // Set the Wi-Fi name for your ESP8266 AP
const char* password = "12345678"; // Set a password for your ESP8266 AP

// Pins
#define MQ_SENSOR_PIN A0
#define DHT_PIN D2
#define ONE_WIRE_BUS D3
#define PULSE_SENSOR_PIN D5

// DHT Sensor
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// DS18B20 Sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Pulse Sensor (initialize with pin)
PulseSensorPlayground pulseSensor(PULSE_SENSOR_PIN);
int pulseRate = 0;  // Pulse rate in BPM

// Web server
ESP8266WebServer server(80);

// Variables for storing sensor data
float mqGasValue = 0.0;
float dhtTemperature = 0.0, dhtHumidity = 0.0;
float ds18b20Temperature = 0.0;

// Setup
void setup() {
  Serial.begin(115200);

  // Set up ESP8266 as an Access Point
  WiFi.softAP(ssid, password); // Start Access Point with SSID and password
  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP()); // Print the IP address of the AP

  // Initialize sensors
  dht.begin();
  sensors.begin();

  // Initialize Pulse Sensor
  pulseSensor.begin();  // No need to pass the pin here, as it's already specified in the constructor

  // Web server setup
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
  // MQ Gas Sensor (assuming the sensor provides a voltage signal that can be read via analog)
  mqGasValue = analogRead(MQ_SENSOR_PIN);
  Serial.print("MQ Gas Value: ");
  if (mqGasValue < 1500) {
    Serial.println("Normal Gas Level Detected");  // Display normal for low values
  } else {
    Serial.println("High Gas Level Detected");
  }

  // DHT Temperature and Humidity
  dhtTemperature = dht.readTemperature();
  dhtHumidity = dht.readHumidity();
  if (isnan(dhtTemperature)) {
    dhtTemperature = 0.0;
    Serial.println("Error reading DHT temperature!");
  }
  if (isnan(dhtHumidity)) {
    dhtHumidity = 0.0;
    Serial.println("Error reading DHT humidity!");
  }
  
  // Print DHT11 temperature and humidity values in the terminal
  Serial.print("DHT11 Temperature: ");
  Serial.print(dhtTemperature);
  Serial.println(" °C");

  Serial.print("DHT11 Humidity: ");
  Serial.print(dhtHumidity);
  Serial.println(" %");

  // DS18B20 Temperature
  sensors.requestTemperatures();
  ds18b20Temperature = sensors.getTempCByIndex(0);
  if (isnan(ds18b20Temperature)) {
    ds18b20Temperature = 0.0;
    Serial.println("Error reading DS18B20 temperature!");
  }

  // Print DS18B20 temperature value in the terminal
  Serial.print("DS18B20 Temperature: ");
  Serial.print(ds18b20Temperature);
  Serial.println(" °C");

  // Pulse Sensor BPM
  pulseRate = pulseSensor.getBeatsPerMinute();
  if (pulseRate <= 0) {
    pulseRate = 0;
    Serial.println("Error reading pulse rate!");
  } else {
    // Mask pulse rate between 75 and 100 BPM
    if (pulseRate < 100) {
      pulseRate = 75;
    } else if (pulseRate > 100) {
      pulseRate = 100;
    }
  }

  // Print Pulse Rate in the terminal
  Serial.print("Masked Pulse Rate: ");
  Serial.println(pulseRate);
}

// Handle root route
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>Sensor Data</title>";
  html += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css'>";
  html += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css'>";
  html += "<style>";
  html += ".card { border-radius: 15px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); transition: 0.3s; }";
  html += ".card:hover { transform: scale(1.05); box-shadow: 0 8px 16px rgba(0, 0, 0, 0.3); }";
  html += ".icon { font-size: 2rem; color: #007bff; }";
  html += "</style>";
  html += "</head><body class='bg-light'>";
  html += "<div class='container mt-5'>";
  html += "<h1 class='text-center mb-4'>Sensor Readings</h1>";
  html += "<div class='row'>";
  
  // MQ Gas Sensor Card
  html += "<div class='col-md-4'>";
  html += "<div class='card text-center p-3'>";
  html += "<i class='fas fa-gas-pump icon'></i>";
  html += "<h5 class='mt-3'>MQ Gas Sensor</h5>";
  html += "<p class='mb-0'>";
  if (mqGasValue < 500) {
    html += "Normal";
  } else {
    html += "High Gas Level Detected";
  }
  html += "</p>";
  html += "</div></div>";

  // DHT Temperature Card
  html += "<div class='col-md-4'>";
  html += "<div class='card text-center p-3'>";
  html += "<i class='fas fa-thermometer-half icon'></i>";
  html += "<h5 class='mt-3'>DHT11 Temperature</h5>";
  html += "<p class='mb-0'>" + String(dhtTemperature) + " °C</p>";
  html += "</div></div>";

  // DHT Humidity Card
  html += "<div class='col-md-4'>";
  html += "<div class='card text-center p-3'>";
  html += "<i class='fas fa-water icon'></i>";
  html += "<h5 class='mt-3'>DHT11 Humidity</h5>";
  html += "<p class='mb-0'>" + String(dhtHumidity) + " %</p>";
  html += "</div></div>";

  // DS18B20 Temperature Card
  html += "<div class='col-md-4'>";
  html += "<div class='card text-center p-3'>";
  html += "<i class='fas fa-temperature-low icon'></i>";
  html += "<h5 class='mt-3'>DS18B20 Temperature</h5>";
  html += "<p class='mb-0'>" + String(ds18b20Temperature) + " °C</p>";
  html += "</div></div>";

  // Pulse Rate Card
  html += "<div class='col-md-4'>";
  html += "<div class='card text-center p-3'>";
  html += "<i class='fas fa-heartbeat icon'></i>";
  html += "<h5 class='mt-3'>Masked Pulse Rate</h5>";
  html += "<p class='mb-0'>" + String(pulseRate) + " BPM</p>";
  html += "</div></div>";

  html += "</div></div>";
  html += "</body></html>";

  // Send the HTML response
  server.send(200, "text/html", html);
}
