// Pin connected to the rain sensor
const int rainPin = D0;  // Change D1 to the correct pin where the sensor is connected

// Variable to store the rain sensor value
int rainState = 0;

void setup() {
  // Start the serial communication for debugging
  Serial.begin(115200);
  
  // Set the rain sensor pin as input
  pinMode(rainPin, INPUT);
}

void loop() {
  // Read the rain sensor value
  rainState = digitalRead(rainPin);
  
  // If rain is detected (assuming HIGH or LOW indicates rain, check your sensor specifications)
  if (rainState == HIGH) {  // Change to LOW if the sensor gives a LOW signal when it detects rain
    Serial.println("No Rain");
  } else {
    Serial.println("Rain Detected");
  }

  // Delay for a short period to avoid overwhelming the serial monitor
  delay(500);
}
