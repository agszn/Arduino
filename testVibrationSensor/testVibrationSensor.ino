// Pin connected to the vibration sensor
const int vibrationPin = D1;  // Change D1 to the correct pin where the sensor is connected

// Variable to store vibration sensor value
int vibrationState = 0;

void setup() {
  // Start the serial communication for debugging
  Serial.begin(115200);
  
  // Set the vibration sensor pin as input
  pinMode(vibrationPin, INPUT);
}

void loop() {
  // Read the vibration sensor value
  vibrationState = digitalRead(vibrationPin);
  
  // If vibration is detected (assuming HIGH indicates vibration)
  if (vibrationState == HIGH) {
    Serial.println("Vibration Detected!");
  } else {
    Serial.println("No Vibration");
  }

  // Delay for a short period to avoid overwhelming the serial monitor
  delay(500);
}
