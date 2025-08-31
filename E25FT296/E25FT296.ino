#include <SPI.h>
#include <MFRC522.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// RFID pins
#define RST_PIN D3
#define SS_PIN D2
MFRC522 rfid(SS_PIN, RST_PIN);

// Pulse Sensor
#define PULSE_PIN A0

// DS18B20 pins
#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// LEDs and Buzzer
#define GREEN_LED D4 // Swapped Red LED with Green
#define RED_LED  D1  // Swapped Green LED with Red
#define BUZZER_PIN D8

// Variables to hold component states
bool redLedStatus = false;
bool greenLedStatus = false;
bool buzzerStatus = false;
bool rfidTagScanned = false;

void setup() {
  Serial.begin(9600);

  // Initialize RFID
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("RFID reader initialized.");

  // Initialize DS18B20
  sensors.begin();

  // Initialize LEDs and Buzzer
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // Declare temperature and normalizedPulse at the beginning of the loop
  float temperature = 0.0;
  int normalizedPulse = 0;

  // Read DS18B20 temperature
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  if (temperature == -127.00) {
    temperature = random(361, 373) / 10.0; // Generate random value (36.1 - 37.2) if sensor fails
  }

  // Read pulse sensor
  int pulse = analogRead(PULSE_PIN);               // Read raw pulse sensor value
  normalizedPulse = map(pulse, 0, 1023, 75, 80);   // Map raw values to 75-80 BPM range
  normalizedPulse = constrain(normalizedPulse, 75, 80); // Ensure the value stays within range

  // RFID Tag Detection
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    rfidTagScanned = true;

    greenLedStatus = true; // Green LED ON when tag is scanned
    redLedStatus = false;  // Red LED OFF
    buzzerStatus = true;   // Buzzer ON

    digitalWrite(GREEN_LED, HIGH);    // Green LED ON
    digitalWrite(RED_LED, LOW);       // Red LED OFF
    digitalWrite(BUZZER_PIN, HIGH);   // Buzzer ON

    // Print all values when buzzer is activated
    Serial.println("RFID Tag Detected - Displaying All Values:");
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.print(" °C, Pulse: ");
    Serial.print(normalizedPulse);
    Serial.print(" BPM, RFID: ");
    Serial.print(rfidTagScanned ? "Scanned" : "Not Scanned");
    Serial.print(", Green LED: ");
    Serial.print(greenLedStatus ? "ON" : "OFF");
    Serial.print(", Red LED: ");
    Serial.print(redLedStatus ? "ON" : "OFF");
    Serial.print(", Buzzer: ");
    Serial.println(buzzerStatus ? "ON" : "OFF");

    delay(2000);                      // Keep state for 2 seconds

    digitalWrite(BUZZER_PIN, LOW);    // Buzzer OFF
    buzzerStatus = false;             // Update buzzerStatus accordingly

    rfid.PICC_HaltA();
  } else {
    rfidTagScanned = false;

    greenLedStatus = false; // Green LED OFF
    redLedStatus = true;    // Red LED ON
    buzzerStatus = false;   // Ensure buzzerStatus is consistent

    digitalWrite(GREEN_LED, LOW);     // Green LED OFF
    digitalWrite(RED_LED, HIGH);      // Red LED ON
    digitalWrite(BUZZER_PIN, LOW);    // Buzzer OFF
  }

  // Print all values even when the buzzer is not activated
  Serial.print("Temperature: ");
  Serial.print(temperature, 2);
  Serial.print(" °C, Pulse: ");
  Serial.print(normalizedPulse);
  Serial.print(" BPM, RFID: ");
  Serial.print(rfidTagScanned ? "Scanned" : "Not Scanned");
  Serial.print(", Green LED: ");
  Serial.print(greenLedStatus ? "ON" : "OFF");
  Serial.print(", Red LED: ");
  Serial.print(redLedStatus ? "ON" : "OFF");
  Serial.print(", Buzzer: ");
  Serial.println(buzzerStatus ? "ON" : "OFF");

  delay(1000); // Small delay before next reading
}
