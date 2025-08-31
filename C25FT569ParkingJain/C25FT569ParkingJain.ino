#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// Wi-Fi credentials
const char* ssid = "Hachh";
const char* password = "h4c6u7wv2";

// Pins
const int servoPin = D4;     // GPIO2

ESP8266WebServer server(80);
Servo myServo;

void handleTrigger() {
  Serial.println("🔔 Trigger received!");

  // Open gate to 180°
  myServo.write(180);
  Serial.println("🚪 Gate opened");

  delay(15000);  // Wait for 15 seconds (gate remains open)

  // Close gate to 0°
  myServo.write(0);
  Serial.println("🚪 Gate closed");

  // Send response
  server.send(200, "text/plain", "Gate opened for 15 seconds and closed.");
}

void setup() {
  Serial.begin(115200);

  // Attach and reset servo
  myServo.attach(servoPin);
  myServo.write(0); // Start at closed position

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("🔌 Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Connected to WiFi!");
  Serial.print("🌐 Access this URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/trigger");

  // Define route
  server.on("/trigger", handleTrigger);
  server.begin();
  Serial.println("🚀 Web server started.");
}

void loop() {
  server.handleClient();
}
