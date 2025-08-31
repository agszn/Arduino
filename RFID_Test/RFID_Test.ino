#include <SPI.h>
#include <MFRC522.h>

// Define pins for RFID
#define RST_PIN D3 // Reset pin
#define SS_PIN D4  // SDA pin (Slave Select)

// Create an MFRC522 instance
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200); // Initialize serial communication
  SPI.begin();          // Init SPI bus
  rfid.PCD_Init();      // Init MFRC522 module
  Serial.println("Place your RFID card near the reader...");
}

void loop() {
  // Check if a card is present
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Check if the card can be read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print UID of the card
  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Halt PICC
  rfid.PICC_HaltA();
}
