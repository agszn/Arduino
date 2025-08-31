#include <ESP8266WiFi.h>

// Define the pin for the gas sensor digital output
#define GAS_SENSOR_PIN D1 // Digital pin

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  
  // Initialize the gas sensor digital input pin
  pinMode(GAS_SENSOR_PIN, INPUT);
  
  // Wait for the Serial Monitor to open
  delay(1000);
  Serial.println("MQ Gas Sensor Digital Input Test");
}

void loop() {
  // Read the digital value from the gas sensor (HIGH or LOW)
  int sensorState = digitalRead(GAS_SENSOR_PIN);

  // Print the sensor state to the Serial Monitor
  if (sensorState == HIGH) {
    Serial.println("Gas detected! (HIGH)");
  } else {
    Serial.println("No gas detected. (LOW)");
  }

  // Add a delay for easier readability in the serial monitor
  delay(1000); // Delay for 1 second
}
