#include <SPI.h>
#include <MFRC522.h>

// RFID pins
#define RST_PIN D3
#define SS_PIN  D4
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Ultrasonic sensor pins
#define TRIG_PIN D5
#define ECHO_PIN D6

// LED pins
#define RED1    D0
#define RED2    D1
#define GREEN1  D2
#define GREEN2  D8

unsigned long lastMillis = 0;
unsigned long interval = 2000;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(RED1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(GREEN2, OUTPUT);

  allLightsOff();
  Serial.println("System Ready: Ultrasonic + RFID");
}

void loop() {
  long distance = readDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Traffic light logic based on distance
  if (distance < 5) {
    setLights(HIGH, LOW, LOW, HIGH);  // Road 1 congested
  } else {
    setLights(LOW, HIGH, HIGH, LOW);  // Road 1 free
  }

  // Add extended delay between ultrasonic readings and check RFID during this time
  rfidDelayCheck(1000);  // 1000ms total delay with RFID polling
}

long readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

void rfidDelayCheck(unsigned long totalDelay) {
  unsigned long start = millis();
  while (millis() - start < totalDelay) {
    if (checkRFID()) {
      // Emergency vehicle → Give priority to Road 1
      setLights(LOW, HIGH, HIGH, LOW);
      Serial.println("EMERGENCY RFID detected!");
      break;  // Optional: exit loop early if RFID is detected
    }
    delay(100);  // Poll RFID every 100ms
  }
}

bool checkRFID() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("RFID Tag UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    mfrc522.PICC_HaltA();
    return true;
  }
  return false;
}

void setLights(bool r1, bool g1, bool r2, bool g2) {
  digitalWrite(RED1, r1);
  digitalWrite(GREEN1, g1);
  digitalWrite(RED2, r2);
  digitalWrite(GREEN2, g2);
}

void allLightsOff() {
  setLights(LOW, LOW, LOW, LOW);
}
