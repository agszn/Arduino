#include <SPI.h>
#include <MFRC522.h>

// Define RFID module pins
#define RST_PIN D1
#define SS_PIN D2

// Define buzzer and LED pins
#define BUZZER_PIN D3
#define LED_PIN D4

MFRC522 rfid(SS_PIN, RST_PIN);  // Create instance of the RFID

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial);

  // Initialize SPI and RFID module
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Place your RFID card near the reader...");

  // Initialize buzzer and LED pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Set buzzer and LED to LOW initially
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // Check if an RFID card is detected
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Verify if the RFID card can be read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Print UID of the card
  Serial.print("Card UID: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Trigger the buzzer and LED when the card is detected
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  delay(500); // Buzzer and LED active for 500ms
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // Halt the card to prepare for the next read
  rfid.PICC_HaltA();
}
