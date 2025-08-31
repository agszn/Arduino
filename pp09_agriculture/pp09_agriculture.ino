#include <ESP8266WiFi.h>
#include <DHT.h>

// Define pins
#define DHTPIN D5            // Digital pin connected to DHT11
#define DHTTYPE DHT11        // DHT 11 type
#define SOIL_MOISTURE_PIN D1 // Digital pin for soil moisture sensor
#define GAS_SENSOR_PIN A0    // Analog pin for MQ gas sensor

// Wi-Fi credentials
const char* ssid = "BSNL3G";
const char* password = "Asdf1234..";

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);
WiFiServer server(80);

void setup() {
  // Start serial communication
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();

  // Set soil moisture pin as input
  pinMode(SOIL_MOISTURE_PIN, INPUT);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client Connected.");
    String request = client.readStringUntil('\r');
    client.flush();

    // Read sensor values
    int soilMoistureValue = digitalRead(SOIL_MOISTURE_PIN); // Read digital soil moisture sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    // Read gas sensor value (analog)
    int gasSensorValue = analogRead(GAS_SENSOR_PIN);
    String gasStatus = gasSensorValue > 300 ? "Gas Detected" : "No Gas Detected"; // Adjust threshold as needed

    // Generate crop suggestions
    String soilCrop = getSoilCrop(soilMoistureValue);
    String tempCrop = getTempCrop(temperature);
    String farmingTips = getFarmingTips(soilMoistureValue, temperature, gasStatus == "Gas Detected");

    // Send HTML response
    client.print(buildHTML(soilMoistureValue, temperature, humidity, gasStatus, soilCrop, tempCrop, farmingTips));
    client.stop();
    Serial.println("Client disconnected.");
  }
}

String getSoilCrop(int soilMoisture) {
  if (soilMoisture == HIGH) {
    return "Rice (Dry soil)";
  } else {
    return "Wheat (Wet soil)";
  }
}

String getTempCrop(float temperature) {
  if (temperature > 35) {
    return "Millet (High temperature)";
  } else if (temperature < 30) {
    return "Potato (Cool temperature)";
  } else {
    return "General crops";
  }
}

String getFarmingTips(int soilMoisture, float temperature, bool gasDetected) {
  String tips = "<ul>";
  if (soilMoisture == HIGH) {
    tips += "<li>Ensure proper drainage to avoid waterlogging in fields.</li>";
    tips += "<li>Use cover crops to improve soil quality.</li>";
  } else {
    tips += "<li>Practice mulching to retain soil moisture.</li>";
    tips += "<li>Consider drip irrigation for efficient water use.</li>";
  }

  if (temperature > 35) {
    tips += "<li>Plant heat-tolerant crops like millet or sorghum.</li>";
    tips += "<li>Use shade nets to protect crops from intense heat.</li>";
  } else if (temperature < 30) {
    tips += "<li>Consider planting cool-weather crops like spinach and potatoes.</li>";
    tips += "<li>Protect crops from frost by using organic mulch.</li>";
  }

  if (gasDetected) {
    tips += "<li>Inspect the area for potential sources of harmful gases.</li>";
    tips += "<li>Improve ventilation or aeration to reduce gas concentration.</li>";
  } else {
    tips += "<li>Maintain proper aeration to ensure healthy soil biology.</li>";
  }

  tips += "</ul>";
  return tips;
}

String buildHTML(int soilMoisture, float temperature, float humidity, String gasStatus, String soilCrop, String tempCrop, String farmingTips) {
  String html = "<!DOCTYPE html><html lang='en'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Sensor Data</title>";
  html += "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css' rel='stylesheet'>";
  html += "<link href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css' rel='stylesheet'>";
  html += "</head>";
  html += "<body class='bg-light'>";
  html += "<div class='container py-5'>";
  html += "<h1 class='text-center mb-4'>Sensor Data</h1>";

  // Soil Moisture Card
  html += "<div class='card mb-3'>";
  html += "<div class='card-body'>";
  html += "<h5 class='card-title'><i class='fas fa-water'></i> Soil Moisture</h5>";
  html += "<p class='card-text'>Status: " + String(soilMoisture == HIGH ? "Dry" : "Wet") + "</p>";
  html += "<p class='card-text'>Suggested Crop: " + soilCrop + "</p>";
  html += "</div></div>";

  // Temperature Card
  html += "<div class='card mb-3'>";
  html += "<div class='card-body'>";
  html += "<h5 class='card-title'><i class='fas fa-thermometer-half'></i> Temperature</h5>";
  html += "<p class='card-text'>Value: " + String(temperature) + " °C</p>";
  html += "<p class='card-text'>Suggested Crop: " + tempCrop + "</p>";
  html += "</div></div>";

  // Humidity Card
  html += "<div class='card mb-3'>";
  html += "<div class='card-body'>";
  html += "<h5 class='card-title'><i class='fas fa-tint'></i> Humidity</h5>";
  html += "<p class='card-text'>Value: " + String(humidity) + " %</p>";
  html += "</div></div>";

  // Gas Sensor Card
  html += "<div class='card mb-3'>";
  html += "<div class='card-body'>";
  html += "<h5 class='card-title'><i class='fas fa-wind'></i> Gas Sensor</h5>";
  html += "<p class='card-text'>Status: " + gasStatus + "</p>";
  html += "</div></div>";

  // Organic Farming Tips Card
  html += "<div class='card mb-3'>";
  html += "<div class='card-body'>";
  html += "<h5 class='card-title'><i class='fas fa-seedling'></i> Organic Farming Tips</h5>";
  html += farmingTips;
  html += "</div></div>";

  html += "</div>";
  html += "</body></html>";

  return html;
}
