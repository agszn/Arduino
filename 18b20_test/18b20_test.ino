#include <OneWire.h>
#include <DallasTemperature.h>

// Pin connected to the DS18B20 data line
#define ONE_WIRE_BUS D2  // GPIO4 on NodeMCU

// Setup oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  Serial.println("Initializing DS18B20 Sensor...");

  // Start the DallasTemperature library
  sensors.begin();
}

void loop() {
  // Request temperature readings
  sensors.requestTemperatures();

  // Fetch and print the temperature in Celsius
  float temperatureC = sensors.getTempCByIndex(0); // Get temperature from the first sensor

  if (temperatureC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: DS18B20 sensor not found!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");
  }

  // Wait 2 seconds before next reading
  delay(2000);
}
