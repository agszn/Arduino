int irSensorPin = 2;  // Digital pin connected to OUT pin of IR sensor
int sensorValue = 0;

void setup() {
  pinMode(irSensorPin, INPUT);
  Serial.begin(9600);  // Start serial communication
}

void loop() {
  sensorValue = digitalRead(irSensorPin);  // Read sensor output
  
  if (sensorValue == LOW) {
    Serial.println("Object Detected");
  } else {
    Serial.println("No Object");
  }
  
  delay(500);  // Wait half a second
}
