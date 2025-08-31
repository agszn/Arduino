// Define the digital pins connected to flex sensors
#define FLEX_PIN_1 D1
#define FLEX_PIN_2 D2
#define FLEX_PIN_3 D3
#define FLEX_PIN_4 D4
#define FLEX_PIN_5 D5

// Define threshold to determine whether the sensor is bent or not
#define BEND_THRESHOLD 500 // You can adjust this value based on your flex sensor's behavior

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Set flex sensor pins as input
  pinMode(FLEX_PIN_1, INPUT);
  pinMode(FLEX_PIN_2, INPUT);
  pinMode(FLEX_PIN_3, INPUT);
  pinMode(FLEX_PIN_4, INPUT);
  pinMode(FLEX_PIN_5, INPUT);
}

void loop() {
  // Read the digital values from each flex sensor
  int sensorValue1 = digitalRead(FLEX_PIN_1);
  int sensorValue2 = digitalRead(FLEX_PIN_2);
  int sensorValue3 = digitalRead(FLEX_PIN_3);
  int sensorValue4 = digitalRead(FLEX_PIN_4);
  int sensorValue5 = digitalRead(FLEX_PIN_5);

  // Check if the sensor's value exceeds the threshold (indicating bend)
  if (sensorValue1 == HIGH) {
    Serial.println("Flex Sensor 1 is bent.");
  } else {
    Serial.println("Flex Sensor 1 is in default position.");
  }

  if (sensorValue2 == HIGH) {
    Serial.println("Flex Sensor 2 is bent.");
  } else {
    Serial.println("Flex Sensor 2 is in default position.");
  }

  if (sensorValue3 == HIGH) {
    Serial.println("Flex Sensor 3 is bent.");
  } else {
    Serial.println("Flex Sensor 3 is in default position.");
  }

  if (sensorValue4 == HIGH) {
    Serial.println("Flex Sensor 4 is bent.");
  } else {
    Serial.println("Flex Sensor 4 is in default position.");
  }

  if (sensorValue5 == HIGH) {
    Serial.println("Flex Sensor 5 is bent.");
  } else {
    Serial.println("Flex Sensor 5 is in default position.");
  }

  // Wait for half a second before reading again
  delay(500);
}
