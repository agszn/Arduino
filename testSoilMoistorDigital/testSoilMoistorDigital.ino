#include <ESP8266WiFi.h>

// Define the pin for the soil moisture sensor
#define SOIL_MOISTURE_PIN D1  // Digital pin

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  
  // Initialize the soil moisture sensor pin as input
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  
  // Wait for the Serial Monitor to open
  delay(1000);
  Serial.println("Soil Moisture Sensor Test");
}

void loop() {
  // Read the digital value from the soil moisture sensor
  int moistureStatus = digitalRead(SOIL_MOISTURE_PIN);

  // Print the moisture status to the Serial Monitor
  if (moistureStatus == HIGH) {
    Serial.println("Soil is wet (HIGH)");
  } else {
    Serial.println("Soil is dry (LOW)");
  }

  // Add a delay for easier readability in the serial monitor
  delay(1000); // Delay for 1 second
}
