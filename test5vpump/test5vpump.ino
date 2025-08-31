const int relay = 4; // Pin connected to the relay

void setup() {
  Serial.begin(115200); // Initialize serial communication
  pinMode(relay, OUTPUT); // Set relay pin as output
}

void loop() {
  digitalWrite(relay, HIGH); // Turn on the relay
  Serial.println("Pump is on"); // Print message to serial monitor
  delay(5000); // Wait for 5 seconds

  digitalWrite(relay, LOW); // Turn off the relay
  Serial.println("Pump is off"); // Print message to serial monitor
  delay(5000); // Wait for 5 seconds
}
