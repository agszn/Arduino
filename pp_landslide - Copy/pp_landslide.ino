#include <Servo.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP_Mail_Client.h>

// Wi-Fi credentials
const char* ssid = "BSNL3G";
const char* password = "Asdf1234..";

// Static IP configuration
IPAddress local_IP(0, 0, 0, 0);  // Change to match your network
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Gmail SMTP server credentials
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define EMAIL_SENDER "shrinidhijoshi1363@gmail.com"
#define EMAIL_PASSWORD "edta qhwr olla bkpo"
#define EMAIL_RECIPIENT "rekhamgudiyavar@gmail.com"

// Create SMTP session and configuration
SMTPSession smtp;
Session_Config config;
SMTP_Message message;

// Create web server instance
ESP8266WebServer server(80);

// Pin assignments
#define X_PIN D5       // GPIO 14 for Accelerometer X
#define Y_PIN D6       // GPIO 12 for Accelerometer Y
#define Z_PIN D7       // GPIO 13 for Accelerometer Z
#define SOIL_PIN A0    // Analog pin for Soil Moisture Sensor
#define DHT_PIN D2     // GPIO 4 for DHT11
#define BUZZER_PIN D8  // GPIO 15 for Buzzer
#define SERVO_PIN D4   // GPIO 2 for Servo Motor

// Create objects
Servo myServo;
DHT dht(DHT_PIN, DHT11);

// Sensor values
int xState, yState, zState, soilMoisture;
float temperature;
bool buzzerStatus = false;

// Function prototypes
void smtpCallback(SMTP_Status status);
void sendEmail();
void handleRoot();
void handleSensorData();

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize sensor and actuator pins
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize Servo motor
  myServo.attach(SERVO_PIN);  // Attach servo to D4 (GPIO 2)

  // Initialize DHT11 sensor
  dht.begin();

  // Configure static IP
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure static IP");
  }

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.println(WiFi.localIP());

  // Configure SMTP session
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = EMAIL_SENDER;
  config.login.password = EMAIL_PASSWORD;

  smtp.callback(smtpCallback);  // Assign the callback function for SMTP

  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/sensor", handleSensorData);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  // Handle web server requests
  server.handleClient();

  // Read accelerometer values
  xState = digitalRead(X_PIN);
  yState = digitalRead(Y_PIN);
  zState = digitalRead(Z_PIN);

  // Read soil moisture sensor value
  soilMoisture = analogRead(SOIL_PIN);

  // Read temperature from DHT11
  temperature = dht.readTemperature();

  // Check conditions
  if (xState == 1 && yState == 1 && zState == 1 && temperature > 35 && soilMoisture < 1000) {
    // Rotate the servo motor to 90 degrees
    myServo.write(90);

    // Start the buzzer
    digitalWrite(BUZZER_PIN, HIGH);
    buzzerStatus = true;

    // Send email
    sendEmail();

    // Wait to avoid spamming emails
    delay(10000);  // 10-second delay
  } else {
    // Reset servo motor to 0 degrees if conditions are not met
    myServo.write(0);

    // Turn off the buzzer
    digitalWrite(BUZZER_PIN, LOW);
    buzzerStatus = false;
  }

  // Wait before next loop
  delay(1000);
}

// Callback function for SMTP debugging
void smtpCallback(SMTP_Status status) {
  Serial.println(status.info());
}

// Handle the root webpage
void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>IoT Project Dashboard</title>
      <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css" rel="stylesheet">
      <style>
        body {
          background-color: #f8f9fa;
        }
        .container {
          margin-top: 20px;
        }
        .sensor-card {
          margin-bottom: 20px;
        }
        .badge {
          font-size: 1rem;
        }
      </style>
      <script>
        function updateData() {
          fetch('/sensor')
            .then(response => response.json())
            .then(data => {
              document.getElementById('temp').innerText = data.temperature + " °C";
              document.getElementById('moisture').innerText = data.soilMoisture + " %";
              document.getElementById('accelerometer').innerText = data.x + ", " + data.y + ", " + data.z;
              document.getElementById('buzzer').innerText = data.buzzerStatus ? "ON" : "OFF";
              document.getElementById('buzzer').className = data.buzzerStatus ? "badge bg-success" : "badge bg-danger";
            });
        }
        setInterval(updateData, 1000); // Auto-update every second
      </script>
    </head>
    <body>
      <div class="container">
        <h1 class="text-center mb-4">LandSlide Detection</h1>
        <div class="row">
          <div class="col-lg-6 col-md-12">
            <h3>Sensor Data</h3>
            <div class="card sensor-card">
              <div class="card-body">
                <h5 class="card-title">Temperature</h5>
                <p class="card-text">
                  <span id="temp" class="badge bg-secondary">--</span>
                </p>
              </div>
            </div>
            <div class="card sensor-card">
              <div class="card-body">
                <h5 class="card-title">Soil Moisture</h5>
                <p class="card-text">
                  <span id="moisture" class="badge bg-secondary">--</span>
                </p>
              </div>
            </div>
            <div class="card sensor-card">
              <div class="card-body">
                <h5 class="card-title">Accelerometer (X, Y, Z)</h5>
                <p class="card-text">
                  <span id="accelerometer" class="badge bg-secondary">--</span>
                </p>
              </div>
            </div>
            <div class="card sensor-card">
              <div class="card-body">
                <h5 class="card-title">Buzzer Status</h5>
                <p class="card-text">
                  <span id="buzzer" class="badge bg-secondary">--</span>
                </p>
              </div>
            </div>
          </div>
          <div class="col-lg-6 col-md-12">
            <h3>Team Members</h3>
            <div class="row g-2">
              <div class="col-6">
                <div class="card text-center">
                  <div class="card-body">
                    <h5 class="card-title">Amruta </h5>
                    <p class="card-text">2BA21EC015</p>
                  </div>
                </div>
              </div>
              <div class="col-6">
                <div class="card text-center">
                  <div class="card-body">
                    <h5 class="card-title">Kartik </h5>
                    <p class="card-text">2BA21EC037</p>
                  </div>
                </div>
              </div>
              <div class="col-6">
                <div class="card text-center">
                  <div class="card-body">
                    <h5 class="card-title">Rekha </h5>
                    <p class="card-text">2BA21EC078</p>
                  </div>
                </div>
              </div>
              <div class="col-6">
                <div class="card text-center">
                  <div class="card-body">
                    <h5 class="card-title">Shrinidhi </h5>
                    <p class="card-text">2BA21EC096</p>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}


// Handle sensor data endpoint
void handleSensorData() {
  String json = "{";
  json += "\"temperature\":" + String(temperature) + ",";
  json += "\"soilMoisture\":" + String(soilMoisture) + ",";
  json += "\"x\":" + String(xState) + ",";
  json += "\"y\":" + String(yState) + ",";
  json += "\"z\":" + String(zState) + ",";
  json += "\"buzzerStatus\":" + String(buzzerStatus ? "true" : "false");
  json += "}";
  server.send(200, "application/json", json);
}

// Function to send an email
void sendEmail() {
  // Configure email message
  message.sender.name = "NodeMCU Email Bot";
  message.sender.email = EMAIL_SENDER;
  message.subject = "Alert: Conditions Met on NodeMCU!";
  message.text.content = "Alert: The conditions for the accelerometer, DHT11, and soil moisture sensor have been met. Actions have been triggered. Landslide Posiblity vacate area";

  message.addRecipient("Recipient", EMAIL_RECIPIENT);

  // Connect to SMTP server and send the email
  if (!smtp.connect(&config)) {
    Serial.printf("Failed to connect to SMTP server: %s\n", smtp.errorReason().c_str());
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.printf("Failed to send email: %s\n", smtp.errorReason().c_str());
  } else {
    Serial.println("Email sent successfully!");
  }

  smtp.closeSession();  // Close the session
}
