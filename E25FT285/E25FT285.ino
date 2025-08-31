#include <DHT.h>

// Define DHT settings
#define DHTPIN D2        // Pin connected to DHT11 data
#define DHTTYPE DHT11    // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);

// Define MQ sensor settings
#define MQ_PIN A0        // Analog pin connected to MQ sensor

// Define Buzzer settings
#define BUZZER_PIN D3    // Pin connected to buzzer

// Threshold values for predictions
#define TEMPERATURE_THRESHOLD 35.0   // High temperature threshold (30°C)
#define HUMIDITY_THRESHOLD 80.0      // High humidity threshold (80%)
#define GAS_THRESHOLD 500            // MQ sensor threshold for gas detection

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Initialize buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  Serial.println("DHT11, MQ sensor, and Buzzer setup complete.");
}

void loop() {
  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read gas level from MQ sensor
  int gasLevel = analogRead(MQ_PIN);

  // Check if the readings are valid and send to Python
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error: Failed to read from DHT sensor!");
  } else {
    // Send sensor data to Python as a formatted string
    Serial.print("Temperature:");
    Serial.print(temperature);
    Serial.print(",Humidity:");
    Serial.print(humidity);
    Serial.print(",GasLevel:");
    Serial.println(gasLevel);

    // Initialize buzzer status
    bool buzzerOn = false;

    // Prediction logic based on sensor values
    if (temperature > TEMPERATURE_THRESHOLD) {
      Serial.println("Prediction: High Temperature - Potential for drought or forest fire.");
      buzzerOn = true;
    }

    if (humidity > HUMIDITY_THRESHOLD) {
      Serial.println("Prediction: High Humidity - Possible flood or cyclone.");
      buzzerOn = true;
    }

    if (gasLevel > GAS_THRESHOLD) {
      Serial.println("Prediction: Gas Levels High - Risk of fire or hazardous conditions.");
      buzzerOn = true;
    }

    // Additional recommendation logic
    if (temperature > 40.0 && humidity < 30.0) {
      Serial.println("Recommendation: Stay hydrated and avoid outdoor activities due to high heat.");
    }

    if (humidity > 90.0 && gasLevel > 600) {
      Serial.println("Recommendation: Be alert for possible forest fire or flood.");
    }

    // Control the buzzer
    if (buzzerOn) {
      digitalWrite(BUZZER_PIN, HIGH); // Turn buzzer on
    } else {
      digitalWrite(BUZZER_PIN, LOW);  // Turn buzzer off
    }
  }

  // Delay before next reading
  delay(2000); // 2 seconds
}
