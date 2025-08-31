#include <ESP8266WiFi.h>

// Define pin for the gas sensor
#define GAS_SENSOR_PIN A0 // Analog pin

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  
  // Initialize the gas sensor pin
  pinMode(GAS_SENSOR_PIN, INPUT);
  
  // Wait for the Serial Monitor to open
  delay(1000);
  Serial.println("MQ Gas Sensor Test");
}

void loop() {
  // Read the analog value from the gas sensor
  int sensorValue = analogRead(GAS_SENSOR_PIN);

  // Print the sensor value to the Serial Monitor
  Serial.print("Gas Sensor Value: ");
  Serial.println(sensorValue);
  
  // Add a delay for easier readability in the serial monitor
  delay(1000); // Delay for 1 second
}
