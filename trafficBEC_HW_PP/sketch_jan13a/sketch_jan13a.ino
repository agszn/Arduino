// Define pins for ultrasonic sensors
#define TRIG1 D1
#define ECHO1 D2
#define TRIG2 D3
#define ECHO2 D4

// Define pins for LEDs
#define GREEN_LED1 D5
#define RED_LED1 D6
#define GREEN_LED2 D7
#define RED_LED2 D8

// Threshold distance in cm
#define THRESHOLD 5

// Variables to store distances
long duration1, distance1;
long duration2, distance2;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set ultrasonic sensor pins
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  // Set LED pins
  pinMode(GREEN_LED1, OUTPUT);
  pinMode(RED_LED1, OUTPUT);
  pinMode(GREEN_LED2, OUTPUT);
  pinMode(RED_LED2, OUTPUT);

  // Ensure all LEDs are off at startup
  digitalWrite(GREEN_LED1, LOW);
  digitalWrite(RED_LED1, LOW);
  digitalWrite(GREEN_LED2, LOW);
  digitalWrite(RED_LED2, LOW);
}

long measureDistance(int trigPin, int echoPin) {
  // Send a 10-microsecond pulse to the TRIG pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the time it takes for the echo to return
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in cm
  return duration * 0.034 / 2;
}

void loop() {
  // Measure distances from both sensors
  distance1 = measureDistance(TRIG1, ECHO1);
  distance2 = measureDistance(TRIG2, ECHO2);

  // Debug: Print distances
  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print(" cm, Distance 2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  // Check for traffic density (priority only if distance is <= THRESHOLD)
  if (distance1 <= THRESHOLD) {
    // Lane 1 priority
    digitalWrite(GREEN_LED1, HIGH);
    digitalWrite(RED_LED1, LOW);
    digitalWrite(GREEN_LED2, LOW);
    digitalWrite(RED_LED2, HIGH);
    Serial.println("Lane 1 Priority: GREEN_LED1 ON, RED_LED2 ON");
    delay(5000);
  } else if (distance2 <= THRESHOLD) {
    // Lane 2 priority
    digitalWrite(GREEN_LED2, HIGH);
    digitalWrite(RED_LED2, LOW);
    digitalWrite(GREEN_LED1, LOW);
    digitalWrite(RED_LED1, HIGH);
    Serial.println("Lane 2 Priority: GREEN_LED2 ON, RED_LED1 ON");
    delay(5000);
  } else {
    // Default sequence when no priority is required
    digitalWrite(GREEN_LED1, HIGH);
    digitalWrite(RED_LED2, HIGH);
    digitalWrite(RED_LED1, LOW);
    digitalWrite(GREEN_LED2, LOW);
    Serial.println("Default Sequence: GREEN_LED1 ON, RED_LED2 ON");
    delay(2000);

    digitalWrite(GREEN_LED1, LOW);
    digitalWrite(RED_LED1, HIGH);
    digitalWrite(GREEN_LED2, HIGH);
    digitalWrite(RED_LED2, LOW);
    Serial.println("Default Sequence: GREEN_LED2 ON, RED_LED1 ON");
    delay(2000);
  }
}
