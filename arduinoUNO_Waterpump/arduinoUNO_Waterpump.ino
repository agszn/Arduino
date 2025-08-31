// Pin definition
const int relayPin = 7;  // Pin connected to relay module

void setup() {
  // Set the relay pin as OUTPUT
  pinMode(relayPin, OUTPUT);

  // Start with the pump off
  digitalWrite(relayPin, LOW);

  // Serial Monitor for debugging
  Serial.begin(9600);
  Serial.println("Relay Control Example: Pump via Relay");
}

void loop() {
  // Turn the pump ON
  digitalWrite(relayPin, HIGH);  // Relay closes, pump turns on
  Serial.println("Pump ON");
  delay(5000);  // Pump runs for 5 seconds

  // Turn the pump OFF
  digitalWrite(relayPin, LOW);  // Relay opens, pump turns off
  Serial.println("Pump OFF");
  delay(5000);  // Pump remains off for 5 seconds
}
