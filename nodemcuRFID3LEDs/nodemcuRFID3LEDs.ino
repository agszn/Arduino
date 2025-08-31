#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D2
#define RST_PIN D1

MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance

// LED Pins
const int RED_LED = D3;
const int YELLOW_LED = D4;
const int GREEN_LED = D8;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Set all LEDs to LOW (default state)
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);

  Serial.println("Place your RFID tag near the reader...");
}

void loop() {
  // Wait for an RFID tag
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print UID
  Serial.print("UID tag: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Red → Yellow → Green LED sequence
  digitalWrite(RED_LED, HIGH);  // Turn Red LED ON
  delay(10000);  // Wait for 10 seconds
  digitalWrite(RED_LED, LOW);  // Turn Red LED OFF

  digitalWrite(YELLOW_LED, HIGH);  // Turn Yellow LED ON
  delay(15000);  // Wait for 15 seconds
  digitalWrite(YELLOW_LED, LOW);  // Turn Yellow LED OFF

  digitalWrite(GREEN_LED, HIGH);  // Turn Green LED ON
  Serial.println("Scan Success");

  delay(10000);  // Wait for 10 seconds with Green LED on

  // Reset all LEDs to LOW after Green LED is on
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);

  // Halt PICC and stop encryption on PCD
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
