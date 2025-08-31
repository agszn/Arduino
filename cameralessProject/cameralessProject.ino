#include <SoftwareSerial.h>
#include <DHT.h>

// Define RX and TX pins for GPS module
SoftwareSerial gpsSerial(12, 14); // (RX, TX) -> D6, D5 on NodeMCU

// Define DHT11 sensor pin and type
#define DHTPIN 4   // D2 on NodeMCU
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Define button pin
#define BUTTON_PIN 5 // D1 on NodeMCU

void setup() {
    Serial.begin(115200);  // Serial monitor for debugging
    gpsSerial.begin(9600); // GPS module baud rate
    dht.begin();           // Initialize DHT sensor
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with pull-up resistor
    Serial.println("System Initialized...");
}

void loop() {
    // Read GPS data
    while (gpsSerial.available()) {
        char c = gpsSerial.read();
        Serial.print(c); // Print GPS data to Serial Monitor
    }
    
    // Read DHT11 sensor data
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    if (!isnan(temperature) && !isnan(humidity)) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print("°C, Humidity: ");
        Serial.print(humidity);
        Serial.println("%");
    } else {
        Serial.println("Failed to read from DHT sensor!");
    }

    // Check if button is pressed
    if (digitalRead(BUTTON_PIN) == LOW) {
        Serial.println("Button Pressed!");
        delay(500); // Debounce delay
    }
    
    delay(2000); // Delay between sensor readings
}
