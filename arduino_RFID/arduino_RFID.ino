#include <SPI.h>
#include <MFRC522.h>

// Define pins for the RFID module
#define RST_PIN 9  // Reset pin
#define SS_PIN 10  // Slave select pin

MFRC522 rfid(SS_PIN, RST_PIN); // Create an instance of the MFRC522 class

void setup() {
  Serial.begin(9600); // Initialize serial communication
  SPI.begin();        // Initialize SPI bus
  rfid.PCD_Init();    // Initialize the RFID reader
  Serial.println("Place your RFID card near the reader...");
}

void loop() {
  // Check if a card is present
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Check if we can read the card
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print card UID
  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Halt the card to stop further reading
  rfid.PICC_HaltA();
}
