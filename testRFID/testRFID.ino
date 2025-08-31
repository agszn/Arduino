#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 0  // Reset pin to GPIO0
#define SS_PIN  4  // SDA pin to GPIO4 (D2)

MFRC522 rfid(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(9600);
  SPI.begin();  // Init SPI bus
  rfid.PCD_Init();  // Init RFID reader
  Serial.println("Scan an RFID card...");
}

void loop() {
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }
  // Verify if the card can be read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print UID of the RFID tag
  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Halt the card to stop further communication
  rfid.PICC_HaltA();
}
