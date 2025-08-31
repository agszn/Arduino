// Define the analog pin where the sensor is connected
const int sensorPin = A0;  // Soil moisture sensor connected to A0
int sensorValue;           // Variable to store sensor reading

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("Soil Moisture Sensor Test");
  delay(2000);
}

void loop() {
  // Read the analog value from the sensor
  sensorValue = analogRead(sensorPin);
  
  // Map the sensor value to a percentage (optional)
  int moisturePercent = map(sensorValue, 0, 1023, 100, 0); // 100% = Wet, 0% = Dry
  
  // Display the raw value and percentage on the Serial Monitor
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");
  
  // Add delay for readability
  delay(1000);
}
