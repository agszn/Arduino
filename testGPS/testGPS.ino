#include <SoftwareSerial.h>

// Define RX and TX pins for GPS module
SoftwareSerial gpsSerial(12, 14); // (RX, TX) -> D6, D5 on NodeMCU

void setup() {
    Serial.begin(115200);  // Serial monitor for debugging
    gpsSerial.begin(9600); // GPS module baud rate
    Serial.println("GPS module initialized...");
}

void loop() {
    while (gpsSerial.available()) {
        char c = gpsSerial.read();
        Serial.print(c); // Print GPS data to Serial Monitor
    }
}
