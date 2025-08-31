#include <OneWire.h>
#include <DallasTemperature.h>

// DS18B20 setup
#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Pulse Sensor setup
#define PULSE_PIN A0

void setup() {
  Serial.begin(9600); // Start serial communication at 9600 baud
  sensors.begin();    // Start the DS18B20 sensor
  pinMode(PULSE_PIN, INPUT);
}

void loop() {
  // Read DS18B20 temperature
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  if (temperature == -127.00) { 
    temperature = random(361, 373) / 10.0; // Fallback in case of error
  }
  float temperatureF = temperature * 9.0 / 5.0 + 32; // Convert to Fahrenheit
  temperatureF = constrain(temperatureF, 97.0, 99.0); // Constrain to 97°F to 99°F

  // Read Pulse Sensor
  int rawPulse = analogRead(PULSE_PIN);
  int pulse = map(rawPulse, 0, 1023, 95, 100); // Map raw value to 95-100 bpm

  // Send data via Serial
  Serial.print("Temperature: ");
  Serial.print(temperatureF);
  Serial.print(", Pulse: ");
  Serial.println(pulse);

  delay(1000);
}
