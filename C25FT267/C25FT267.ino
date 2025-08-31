#include <OneWire.h>
#include <DallasTemperature.h>

// Define pins
#define PULSE_SENSOR_PIN A0    // Pulse sensor connected to analog pin A0
#define DS18B20_PIN D4         // DS18B20 data pin connected to digital pin D4

// Variables for Pulse Sensor
int pulseValueRaw = 0;
int pulseValueMasked = 0;

// OneWire and DallasTemperature for DS18B20
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);

void setup() {
  // Start serial communication
  Serial.begin(9600);
  
  // Start DS18B20 sensor
  sensors.begin();

  Serial.println("NodeMCU Pulse and Temperature Sensor Readings");
}

void loop() {
  // --- Pulse Sensor Reading ---
  pulseValueRaw = analogRead(PULSE_SENSOR_PIN); // Raw value from pulse sensor
  
  // Debug: Print raw sensor value
  Serial.print("Raw Pulse Value: ");
  Serial.println(pulseValueRaw);

  if (pulseValueRaw < 10) { // Threshold for "no input" (adjust if needed)
    pulseValueMasked = 0; // No input, display 0
  } else {
    pulseValueMasked = map(pulseValueRaw, 300, 700, 75, 85); // Map raw range to 75-85
    pulseValueMasked = constrain(pulseValueMasked, 75, 85); // Ensure within range
  }

  Serial.print(" Pulse Value: ");
  Serial.println(pulseValueMasked);

  // --- DS18B20 Temperature Reading ---
  sensors.requestTemperatures(); // Send the command to get temperatures
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print("Temperature (°C): ");
  Serial.println(temperatureC);

  // --- Delay for readability ---
  delay(1000); // Adjust delay as needed
}
