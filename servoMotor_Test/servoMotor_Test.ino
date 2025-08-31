#include <Servo.h>

// Create a Servo object
Servo myServo;

// Define the servo pin
#define SERVO_PIN D4 // GPIO2

void setup() {
  // Start serial communication
  Serial.begin(115200);
  Serial.println("Servo Motor Test");

  // Attach the servo to the pin
  myServo.attach(SERVO_PIN);

  // Move servo to the initial position
  myServo.write(90); // Middle position
  Serial.println("Servo initialized to 90 degrees");
  delay(1000);
}

void loop() {
  // Move servo to 0 degrees
  Serial.println("Moving to 0 degrees");
  myServo.write(0);
  delay(1000);

  // Move servo to 90 degrees
  Serial.println("Moving to 90 degrees");
  myServo.write(90);
  delay(1000);

  // Move servo to 180 degrees
  Serial.println("Moving to 180 degrees");
  myServo.write(180);
  delay(1000);
}
