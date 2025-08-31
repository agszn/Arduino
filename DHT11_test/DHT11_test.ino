#include <DHT.h>

// Pin where the DHT11 data pin is connected
#define DHTPIN D5 // Replace with the GPIO pin connected to DHT11

// Define the DHT type (DHT11)
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200); // Initialize serial communication
  dht.begin();          // Start the DHT sensor
  Serial.println("DHT11 test started.");
}

void loop() {
  // Read humidity
  float humidity = dht.readHumidity();

  // Read temperature in Celsius
  float temperatureC = dht.readTemperature();

  // Read temperature in Fahrenheit
  float temperatureF = dht.readTemperature(true);

  // Check if readings are valid
  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.print(" °C ");
    Serial.print("(");
    Serial.print(temperatureF);
    Serial.println(" °F)");
  }

  delay(2000); // Wait for 2 seconds before next reading
}
