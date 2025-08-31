#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10      // RFID SS pin
#define RST_PIN 9      // RFID RST pin
#define RFID_BUZZER 7  // RFID-related buzzer
#define SERVO_PIN 6    // Servo signal pin
#define US_TRIG 4      // Ultrasonic Trigger pin
#define US_ECHO 5      // Ultrasonic Echo pin
#define US_BUZZER A3   // Ultrasonic-related buzzer (A3)

MFRC522 rfid(SS_PIN, RST_PIN);
Servo myServo;

// Authorized UIDs
byte card1[4] = {0xB3, 0xE3, 0x02, 0x2F}; // 5 sec
byte card2[4] = {0x43, 0x5A, 0x6D, 0xFA}; // 10 sec

unsigned long lastScanTime = 0;
const unsigned long blockDuration = 15000; // 15 seconds block time

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(RFID_BUZZER, OUTPUT);
  pinMode(US_BUZZER, OUTPUT);
  pinMode(US_TRIG, OUTPUT);
  pinMode(US_ECHO, INPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(0); // Locked

  Serial.println("Place RFID tag near the reader...");
}

void loop() {
  // Always check ultrasonic distance
  checkUltrasonic();

  // Block RFID scanning for 15 seconds after successful scan
  if (millis() - lastScanTime < blockDuration) {
    return;
  }

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Card UID: ");
  for (byte i = 0; i < 4; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (isMatch(rfid.uid.uidByte, card1)) {
    unlockAction(5000, 2, 3);
  } else if (isMatch(rfid.uid.uidByte, card2)) {
    unlockAction(10000, 2, 3);
  } else {
    denyAction();
  }

  lastScanTime = millis();
  rfid.PICC_HaltA();
  delay(1000);
}

bool isMatch(byte *uid, byte *card) {
  for (byte i = 0; i < 4; i++) {
    if (uid[i] != card[i]) {
      return false;
    }
  }
  return true;
}

void unlockAction(unsigned long duration, int minWeight, int maxWeight) {
  Serial.println("Access Granted");
  tone(RFID_BUZZER, 1000, 200);  // Short beep
  myServo.write(180);
  delay(duration);
  myServo.write(0);
  delay(duration);

  int simulatedWeight = random(minWeight, maxWeight + 1);
  Serial.print(" Estimated Weight: ");
  Serial.print(simulatedWeight);
  Serial.println(" grams");
}

void denyAction() {
  Serial.println("Access Denied");
  tone(RFID_BUZZER, 500, 1000); // Long beep
}

void checkUltrasonic() {
  digitalWrite(US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIG, LOW);

  long duration = pulseIn(US_ECHO, HIGH);
  float distance = duration * 0.034 / 2;

  if (distance > 0 && distance < 5.0) {
    tone(US_BUZZER, 2000); // Continuous tone if object is close
  } else {
    noTone(US_BUZZER);
  }
}
