#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP_Mail_Client.h>

// Wi-Fi credentials
const char* ssid = "BSNL3G";
const char* password = "Asdf1234..";

// Static IP configuration
IPAddress local_IP(192, 168, 1, 100);  // Change to match your network
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Gmail SMTP server credentials
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define EMAIL_SENDER "sznvws@gmail.com"
#define EMAIL_PASSWORD "qmib ecqd egdf xaxl"
#define EMAIL_RECIPIENT "sznvws@gmail.com"

// Create SMTP session and configuration
SMTPSession smtp;
Session_Config config;
SMTP_Message message;

// Create web server instance
ESP8266WebServer server(80);

// Pin assignments
#define VIBRATION_PIN D1 // GPIO 5 for Vibration Sensor
#define RAIN_PIN D3      // GPIO 0 for Rain Sensor
#define BUZZER_PIN D8    // GPIO 15 for Buzzer

// Sensor values
bool rainDetected = false;
int vibrationState = 0;
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
  pinMode(VIBRATION_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT_PULLUP); // Rain sensor is active LOW
  pinMode(BUZZER_PIN, OUTPUT);

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

  // Read vibration sensor value
  vibrationState = digitalRead(VIBRATION_PIN);

  // Check rain sensor value (LOW means rain detected)
  rainDetected = digitalRead(RAIN_PIN) == LOW;

  // Check conditions
  if (vibrationState == 1 && rainDetected) {
    // Start the buzzer
    digitalWrite(BUZZER_PIN, HIGH);
    buzzerStatus = true;

    // Send email
    sendEmail();

    // Wait to avoid spamming emails
    delay(10000);  // 10-second delay
  } else {
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
              document.getElementById('buzzer').innerText = data.buzzerStatus ? "ON" : "OFF";
              document.getElementById('buzzer').className = data.buzzerStatus ? "badge bg-success" : "badge bg-danger";
              document.getElementById('vibration').innerText = data.vibration == 1 ? "Detected" : "None";
              document.getElementById('rain').innerText = data.rainDetected ? "Rain Detected" : "No Rain";
            });
        }
        setInterval(updateData, 1000); // Auto-update every second
      </script>
    </head>
    <body>
      <div class="container">
        <h1 class="text-center mb-4">Rain & Vibration Sensor Dashboard</h1>
        <div class="row">
          <div class="col-lg-6 col-md-12">
            <h3>Sensor Data</h3>
            <div class="card sensor-card">
              <div class="card-body">
                <h5 class="card-title">Buzzer Status</h5>
                <p class="card-text">
                  <span id="buzzer" class="badge bg-secondary">--</span>
                </p>
              </div>
            </div>
            <div class="card sensor-card">
              <div class="card-body">
                <h5 class="card-title">Vibration</h5>
                <p class="card-text"><span id="vibration" class="badge bg-secondary">--</span></p>
              </div>
            </div>
            <div class="card sensor-card">
              <div class="card-body">
                <h5 class="card-title">Rain</h5>
                <p class="card-text"><span id="rain" class="badge bg-secondary">--</span></p>
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
  json += "\"buzzerStatus\":" + String(buzzerStatus ? "true" : "false") + ",";
  json += "\"vibration\":" + String(vibrationState) + ",";
  json += "\"rainDetected\":" + String(rainDetected ? "true" : "false");
  json += "}";
  server.send(200, "application/json", json);
}

// Function to send an email
void sendEmail() {
  // Configure email message
  message.sender.name = "NodeMCU Email Bot";
  message.sender.email = EMAIL_SENDER;
  message.subject = "Alert: Rain and Vibration Detected!";
  message.text.content = "Alert: Rain and vibration detected! Actions have been triggered.";

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
