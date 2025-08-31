#include <MFRC522.h>
#include <SPI.h>

// RFID Pins
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
byte emergencyTag[4] = {0x3, 0xE2, 0xF9, 0x2E}; // Replace with actual UID

// Ultrasonic Lane A
#define trigA 2
#define echoA 3

// Ultrasonic Lane B
#define trigB 4
#define echoB 5

// Lane A LEDs
#define greenA 6
#define yellowA 7
#define redA 8

// Lane B LEDs
#define greenB A0
#define yellowB A1
#define redB A2

// Buzzer
#define buzzer A3

bool emergencyDetected = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  // Setup outputs
  pinMode(greenA, OUTPUT);
  pinMode(yellowA, OUTPUT);
  pinMode(redA, OUTPUT);
  pinMode(greenB, OUTPUT);
  pinMode(yellowB, OUTPUT);
  pinMode(redB, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.println("System Ready. Scan RFID to simulate emergency.");
}

void loop() {
  checkRFID();

  if (emergencyDetected) {
    Serial.println("Emergency Mode: Lane B GREEN priority.");
    buzz(3);
    setLights(false);  // Lane A RED
    setLights(true);   // Lane B GREEN
    delay(5000);
    emergencyDetected = false;
  } else {
    normalCycle();
  }
}

void checkRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  byte readUID[4];
  for (byte i = 0; i < 4; i++) {
    readUID[i] = rfid.uid.uidByte[i];
  }

  Serial.print("Scanned UID: ");
  for (byte i = 0; i < 4; i++) {
    Serial.print(readUID[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (memcmp(readUID, emergencyTag, 4) == 0) {
    Serial.println("✅ Emergency RFID detected.");
    emergencyDetected = true;
  } else {
    Serial.println("❌ Not an emergency tag.");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void normalCycle() {
  setLights(true);  // Lane A green
  setLights(false); // Lane B red
  delay(5000);

  yellowPhase(true);
  delay(2000);

  setLights(false); // Lane A red
  setLights(true);  // Lane B green
  delay(5000);

  yellowPhase(false);
  delay(2000);
}

void setLights(bool laneA) {
  if (laneA) {
    digitalWrite(greenA, HIGH);
    digitalWrite(yellowA, LOW);
    digitalWrite(redA, LOW);
    analogWrite(redB, 255);
    analogWrite(greenB, 0);
    analogWrite(yellowB, 0);
  } else {
    digitalWrite(greenA, LOW);
    digitalWrite(yellowA, LOW);
    digitalWrite(redA, HIGH);
    analogWrite(greenB, 255);
    analogWrite(yellowB, 0);
    analogWrite(redB, 0);
  }
}

void yellowPhase(bool laneA) {
  if (laneA) {
    digitalWrite(greenA, LOW);
    digitalWrite(yellowA, HIGH);
    digitalWrite(redA, LOW);
  } else {
    analogWrite(greenB, 0);
    analogWrite(yellowB, 255);
    analogWrite(redB, 0);
  }
}

void buzz(int times) {
  Serial.println("Buzzing...");
  for (int i = 0; i < times; i++) {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    delay(200);
  }
}
