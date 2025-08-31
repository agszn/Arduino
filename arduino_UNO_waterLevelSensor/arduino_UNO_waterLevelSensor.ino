// Define the pin for the water level sensor
const int sensorPin = A0;  // Signal pin connected to A0
int waterLevelValue;       // Variable to store sensor reading

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("Water Level Sensor Test");
  delay(2000);
}

void loop() {
  // Read the analog value from the sensor
  waterLevelValue = analogRead(sensorPin);
  
  // Map the sensor value to a percentage (optional)
  int waterLevelPercent = map(waterLevelValue, 0, 1023, 0, 100); // 0% = Low, 100% = High
  
  // Display the raw value and percentage on the Serial Monitor
  Serial.print("Sensor Value: ");
  Serial.print(waterLevelValue);
  Serial.print(" | Water Level: ");
  Serial.print(waterLevelPercent);
  Serial.println("%");
  
  // Add delay for readability
  delay(1000);
}
