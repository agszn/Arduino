// Define pin connections
#define TRIG_PIN D1 // Pin connected to the Trig pin of the ultrasonic sensor
#define ECHO_PIN D2 // Pin connected to the Echo pin of the ultrasonic sensor

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Configure the trigger and echo pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Ultrasonic sensor test started.");
}

void loop() {
  // Send a 10-microsecond pulse to the TRIG pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the time for the echo to return
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance in centimeters
  float distance = duration * 0.034 / 2;

  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000); // Wait for 1 second before the next measurement
}
