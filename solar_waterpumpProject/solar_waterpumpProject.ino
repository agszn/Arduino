const int pumpRelayPin = 8;      // Relay module control pin
const int soilMoisturePin = A0;  // Soil moisture sensor pin
const int waterLevelPin = A1;    // Water level sensor pin

const int soilDryThreshold = 600;   // Adjust based on sensor readings for dry soil
const int waterLevelThreshold = 200; // Adjust based on sensor readings for sufficient water

void setup() {
  pinMode(pumpRelayPin, OUTPUT);  // Set relay pin as output
  digitalWrite(pumpRelayPin, LOW); // Ensure the relay is off initially
  Serial.begin(9600);             // Initialize serial communication
}

void loop() {
  int soilMoistureValue = analogRead(soilMoisturePin); // Read soil moisture
  int waterLevelValue = analogRead(waterLevelPin);     // Read water level

  // Print sensor values for debugging
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  Serial.print("Water Level: ");
  Serial.println(waterLevelValue);

  // Check conditions to turn on the pump
  if (soilMoistureValue > soilDryThreshold && waterLevelValue > waterLevelThreshold) {
    digitalWrite(pumpRelayPin, LOW); // Turn on the relay (pump ON)
    Serial.println("Pump ON");
  } else {
    digitalWrite(pumpRelayPin, HIGH);  // Turn off the relay (pump OFF)
    Serial.println("Pump OFF");
  }

  delay(1000); // Wait 1 second before next check
}
