#include <SPI.h>
#include <MFRC522.h>

// RFID
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Motor driver pins
#define IN1 4
#define IN2 5
#define IN3 2
#define IN4 3

// LED pins
#define RED_LED A0
#define YELLOW_LED A1
#define GREEN_LED A2

// Buzzer pin
#define BUZZER_PIN A3

// Button pin
#define BUTTON_PIN 8

// Ultrasonic pins
#define TRIG_PIN 7
#define ECHO_PIN 6

bool systemActive = true;
bool waitingForPython = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  turnOffLEDs();
  startMotors();

  Serial.println("System Ready.");
}

void loop() {
  // Toggle system ON/OFF
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(300); // debounce
    systemActive = !systemActive;
    Serial.println(systemActive ? "System Activated" : "System Deactivated");
    delay(1000);
  }

  if (!systemActive) {
    stopMotors();
    turnOffLEDs();
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }

  // Measure distance
  long distance = measureDistance();

  if (distance > 0) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  } else {
    Serial.println("Ultrasonic read failed or out of range.");
  }

  // If very close (<5 cm): buzzer ON
  if (distance > 0 && distance < 5) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // If within 5 to 20 cm: beep buzzer for 2 seconds, then continue
  if (distance >= 5 && distance < 20) {
    Serial.println("Warning: Object detected within 20 cm. Buzzing...");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // RFID scan
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("RFID_SCANNED");
    digitalWrite(RED_LED, HIGH);  // Turn RED ON when RFID scanned
    waitingForPython = true;
    waitForPythonResponse();
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  delay(200); // loop delay to avoid spamming Serial
}

void waitForPythonResponse() {
  unsigned long startTime = millis();
  while (waitingForPython && (millis() - startTime < 5000)) {  // 5-second timeout
    if (Serial.available()) {
      String msg = Serial.readStringUntil('\n');
      msg.trim();
      if (msg == "s") {
        Serial.println("Python replied OK. Motor continues.");

        // Turn off RED LED, then YELLOW ON for 2 seconds
        digitalWrite(RED_LED, LOW);      
        digitalWrite(YELLOW_LED, HIGH);  
        delay(2000);
        digitalWrite(YELLOW_LED, LOW);

        // Then turn on GREEN
        digitalWrite(GREEN_LED, HIGH);   
        delay(2000);
        digitalWrite(GREEN_LED, LOW);

        startMotors();
        delay(2000);
        stopMotors();
      } else if (msg == "n") {
        Serial.println("Python replied NOT_OK. LED sequence.");
        stopMotors();
        ledSequence();  // RED → YELLOW → GREEN
      }
      waitingForPython = false;
    }
  }

  // Timeout handling if no response after 5 seconds
  if (waitingForPython) {
    Serial.println("No response from Python. Timeout.");

    // If no response after 5 seconds, show YELLOW for 3 seconds, then GREEN
    digitalWrite(RED_LED, LOW);  // Ensure RED is turned off
    digitalWrite(YELLOW_LED, HIGH);  // Glow YELLOW for 3 seconds
    delay(3000);
    digitalWrite(YELLOW_LED, LOW);
    stopMotors();
    digitalWrite(GREEN_LED, HIGH);  // Glow GREEN
    delay(2000);
    digitalWrite(GREEN_LED, LOW);
  }
}

void ledSequence() {
  digitalWrite(RED_LED, HIGH);
  delay(1500);
  digitalWrite(YELLOW_LED, HIGH);
  delay(1500);
  digitalWrite(GREEN_LED, HIGH);
  delay(1500);
  turnOffLEDs();
}

void turnOffLEDs() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}

void startMotors() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
  if (duration == 0) return -1;
  return duration / 58.0; // convert to cm
}
