#include <ESP8266WiFi.h>
#include <Wire.h>
#include <DHT.h>

// Wi-Fi credentials
const char* ssid = "BSNL3G";
const char* password = "Asdf1234..";

// ADXL345 I2C address
#define ADXL345_ADDR 0x53

// ADXL345 registers
#define POWER_CTL 0x2D
#define DATAX0    0x32
#define DATAY0    0x34
#define DATAZ0    0x36

// Threshold for detecting rapid changes
#define MOVEMENT_THRESHOLD 200
#define TEMP_THRESHOLD 30.0

// Buzzer pin
#define BUZZER_PIN D6

// DHT11 setup
#define DHT_PIN D7
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

WiFiServer server(80);

int16_t prevX = 0, prevY = 0;
int16_t x = 0, y = 0, z = 0;
float temperature = 0.0, humidity = 0.0;
bool buzzerActive = false;
bool landslideAlert = false;

void setup() {
  Serial.begin(115200);

  // Initialize I2C
  Wire.begin(D2, D1); // SDA=D2 (GPIO4), SCL=D1 (GPIO5)
  
  // Initialize ADXL345
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(POWER_CTL); // Access POWER_CTL register
  Wire.write(0x08);      // Set to measurement mode
  Wire.endTransmission();

  // Initialize DHT11
  dht.begin();

  // Initialize Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wi-Fi connected");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // Read X, Y, Z data from ADXL345
  x = readAxis(DATAX0);
  y = readAxis(DATAY0);
  z = readAxis(DATAZ0);

  // Detect rapid movement
  bool excessiveMovement = abs(x - prevX) > MOVEMENT_THRESHOLD || abs(y - prevY) > MOVEMENT_THRESHOLD;
  prevX = x;
  prevY = y;

  // Read temperature and humidity from DHT11
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Check for landslide conditions
  landslideAlert = excessiveMovement && temperature > TEMP_THRESHOLD;
  
  if (landslideAlert) {
    buzzerActive = true;
    digitalWrite(BUZZER_PIN, HIGH); // Turn buzzer on
    delay(1000);                    // Keep buzzer on for one second
    digitalWrite(BUZZER_PIN, LOW);  // Turn buzzer off
    buzzerActive = false;
  }

  // Serve the web page
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    String request = client.readStringUntil('\r');
    client.flush();

    // Generate HTML response
    String html = "<!DOCTYPE html><html><head>";
    html += "<title>Sensor Data</title>";
    html += "<link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/css/bootstrap.min.css'>";
    html += "<style>";
    html += ".card { margin: 20px auto; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); border-radius: 10px; }";
    html += "body { background-color: #f8f9fa; }";
    html += ".alert { margin: 20px auto; }";
    html += "</style>";
    html += "</head><body>";
    html += "<div class='container'>";
    html += "<h1 class='text-center my-4'>Sensor Data</h1>";

    // Display acceleration data
    html += "<div class='card p-4'>";
    html += "<h5>Acceleration Data</h5>";
    html += "<p><b>X:</b> " + String(x) + "</p>";
    html += "<p><b>Y:</b> " + String(y) + "</p>";
    html += "<p><b>Z:</b> " + String(z) + "</p>";
    html += "</div>";

    // Display environmental data
    html += "<div class='card p-4'>";
    html += "<h5>Environmental Data</h5>";
    html += "<p><b>Temperature:</b> " + String(temperature) + " °C</p>";
    html += "<p><b>Humidity:</b> " + String(humidity) + " %</p>";
    html += "</div>";

    // Display buzzer status
//    html += "<div class='card p-4'>";
//    html += "<h5>Buzzer Status</h5>";
//    html += "<p><b>Status:</b> " + String(buzzerActive ? "Active" : "Inactive") + "</p>";
//    html += "</div>";

    // Display alert if conditions are met
    if (landslideAlert) {
      html += "<div class='alert alert-danger' role='alert'>";
      html += "<h4 class='alert-heading'>Landslide Possible!</h4>";
      html += "<p>High temperature and excessive movement detected. Immediate action is recommended.</p>";
      html += "<hr>";
      html += "<p class='mb-0'><b>Recommendations:</b></p>";
      html += "<ul>";
      html += "<li>Evacuate the area immediately.</li>";
      html += "<li>Stay updated with local authorities and emergency alerts.</li>";
      html += "<li>Avoid steep slopes and areas prone to landslides.</li>";
      html += "</ul>";
      html += "</div>";
    }

    html += "</div>";
    html += "</body></html>";

    // Send response to client
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    client.print(html);
    client.stop();
    Serial.println("Client disconnected");
  }

  delay(2000); // Update interval
}

// Function to read axis data from ADXL345
int16_t readAxis(uint8_t axisReg) {
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(axisReg);
  Wire.endTransmission();
  
  Wire.requestFrom(ADXL345_ADDR, 2);
  int16_t value = Wire.read() | (Wire.read() << 8); // Combine low and high bytes
  return value;
}
