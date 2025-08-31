// Define the pin where the buzzer is connected
#define BUZZER_PIN D6 // Replace with the GPIO pin connected to the buzzer

void setup() {
  // Initialize the serial communication
  Serial.begin(115200);
  
  // Set the buzzer pin as an output
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.println("Buzzer test started.");
}

void loop() {
  // Test: Turn the buzzer on for 1 second and then off for 1 second
  Serial.println("Buzzer ON");
  digitalWrite(BUZZER_PIN, HIGH); // Turn the buzzer on
  delay(1000);                    // Wait for 1 second

  Serial.println("Buzzer OFF");
  digitalWrite(BUZZER_PIN, LOW);  // Turn the buzzer off
  delay(1000);                    // Wait for 1 second

  // Optional: Generate tones if the buzzer supports it
  for (int freq = 500; freq <= 2000; freq += 500) {
    Serial.print("Playing tone at frequency: ");
    Serial.println(freq);
    tone(BUZZER_PIN, freq);       // Generate a tone
    delay(1000);                  // Play for 1 second
  }

  noTone(BUZZER_PIN);             // Stop any tone
  delay(1000);
}
