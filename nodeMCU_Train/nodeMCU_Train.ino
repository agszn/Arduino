#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// Motor pins
const int motorA1 = 5;
const int motorA2 = 6;
const int motorB1 = 7;
const int motorB2 = 8;

// LED pins
const int ledD2 = 2;
const int ledD3 = 3;
const int ledD4 = 4;

// Button pin (A1)
const int buttonPin = A1;

bool motorsRunning = false;

byte lastCard[4];
bool card1Scanned = false;
unsigned long card1Time = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  pinMode(ledD2, OUTPUT);
  pinMode(ledD3, OUTPUT);
  pinMode(ledD4, OUTPUT);

  pinMode(buttonPin, INPUT_PULLUP); // Button pin with internal pull-up

  stopMotors();
  digitalWrite(ledD2, LOW);
  digitalWrite(ledD3, LOW);
  digitalWrite(ledD4, LOW);

  // Simulate current on
  startMotors();
  motorsRunning = true;
  Serial.println("Motors started due to current/power on.");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    byte *currUID = rfid.uid.uidByte;
    unsigned long now = millis();

    if (!card1Scanned) {
      // First card scanned
      memcpy(lastCard, currUID, 4);
      card1Time = now;
      card1Scanned = true;
      Serial.println("First card scanned.");
    } else {
      // Check if different UID and within 2 seconds
      if (memcmp(lastCard, currUID, 4) != 0 && (now - card1Time <= 2000)) {
        Serial.println("Same track, 2 trains scanned.");
        triggerWarningSequence();
      }
      // Reset regardless
      card1Scanned = false;
    }

    // Halt card reading
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  // Reset card1Scanned flag if more than 2 sec passed
  if (card1Scanned && (millis() - card1Time > 2000)) {
    card1Scanned = false;
  }
}

void triggerWarningSequence() {
  digitalWrite(ledD2, HIGH);
  digitalWrite(ledD3, HIGH);

  unsigned long waitStart = millis();
  bool userResponded = false;

  Serial.println("Waiting for button press to respond...");

  while (millis() - waitStart < 10000) {
    if (digitalRead(buttonPin) == LOW) {  // Button pressed (LOW because of INPUT_PULLUP)
      userResponded = true;
      break;
    }
  }

  if (userResponded) {
    Serial.println("Button pressed. Stopping motors and LEDs.");
    stopMotors();
    digitalWrite(ledD2, LOW);
    digitalWrite(ledD3, LOW);
  } else {
    Serial.println("No response. Safety triggered.");
    digitalWrite(ledD4, HIGH);
    stopMotors();
  }
}

void startMotors() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void stopMotors() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
} 
