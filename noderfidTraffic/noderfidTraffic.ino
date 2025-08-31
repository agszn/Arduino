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

// Define pin for the button and buzzer
#define BUTTON_PIN D0
#define BUZZER_PIN A0

// Threshold distance in cm
#define THRESHOLD 5

// Variables to store distances
long duration1, distance1;
long duration2, distance2;

// Variables to store button state and mode
int buttonState = 0;
int lastButtonState = HIGH; // Previous button state
bool emergencyMode = false; // Flag for emergency mode

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

  // Set button and buzzer pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Ensure all LEDs and buzzer are off at startup
  digitalWrite(GREEN_LED1, LOW);
  digitalWrite(RED_LED1, LOW);
  digitalWrite(GREEN_LED2, LOW);
  digitalWrite(RED_LED2, LOW);
  digitalWrite(BUZZER_PIN, LOW);
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
  // Read the button state
  buttonState = digitalRead(BUTTON_PIN);

  // Check for button press
  if (buttonState == LOW && lastButtonState == HIGH) { // Detect button press
    emergencyMode = !emergencyMode; // Toggle emergency mode
    Serial.print("Emergency Mode: ");
    Serial.println(emergencyMode ? "ON" : "OFF");

    if (emergencyMode) {
      // Activate emergency mode
      Serial.print("Scan RFID to redirect to the hospital ");
      Serial.println("ID: 04 A3 12 7C 29 - Redirecting to KLE Hospital");
      digitalWrite(RED_LED2, HIGH);
      digitalWrite(RED_LED1, HIGH);
      digitalWrite(GREEN_LED1, LOW);
      digitalWrite(GREEN_LED2, LOW);
      delay(5000); // Keep buzzer on for 2 seconds

      // Return to default sequence after emergency
      emergencyMode = false;
    }
  }

  // Update last button state
  lastButtonState = buttonState;

  // Skip traffic signal logic if in emergency mode
  if (emergencyMode) {
    return;
  }

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
    delay(3500);

    digitalWrite(GREEN_LED1, LOW);
    digitalWrite(RED_LED1, HIGH);
    digitalWrite(GREEN_LED2, HIGH);
    digitalWrite(RED_LED2, LOW);
    Serial.println("Default Sequence: GREEN_LED2 ON, RED_LED1 ON");
    delay(3500);
  }
}
