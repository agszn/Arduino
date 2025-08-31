// Pin Definitions
const int waterLevelPin = A0;  // Water level sensor connected to A0
const int soilMoisturePin = A1; // Soil moisture sensor connected to A1
const int relayPin = 7;        // Relay to control pump connected to D7

// Sensor thresholds
const int waterLevelThreshold = 5;   // Water level threshold
const int soilMoistureThreshold = 500; // Soil moisture threshold

// Variables to store sensor readings
int waterLevel = 0;         
int soilMoisture = 0;

void setup() {
  // Initialize the relay pin as OUTPUT
  pinMode(relayPin, OUTPUT);
  
  // Ensure the pump is OFF at startup
  digitalWrite(relayPin, LOW);

  // Initialize Serial Monitor for debugging
  Serial.begin(9600);
  Serial.println("Water Level and Soil Moisture Monitoring System");
}

void loop() {
  // Read water level sensor value
  waterLevel = analogRead(waterLevelPin);

  // Read soil moisture sensor value
  soilMoisture = analogRead(soilMoisturePin);

  // Print sensor values to Serial Monitor
  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.print(" | Soil Moisture: ");
  Serial.println(soilMoisture);

  // Control pump based on conditions
  if (waterLevel > waterLevelThreshold && soilMoisture < soilMoistureThreshold) {
    digitalWrite(relayPin, HIGH); // Turn ON the pump
    Serial.println("Pump Status: ON");
  } else {
    digitalWrite(relayPin, LOW); // Turn OFF the pump
    Serial.println("Pump Status: OFF");
  }

  // Small delay for stability
  delay(1000);
}
