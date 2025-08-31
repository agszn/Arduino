// Define pin connections
#define TRIG_PIN D1 // Pin connected to the Trig pin of the ultrasonic sensor
#define ECHO_PIN D2 // Pin connected to the Echo pin of the ultrasonic sensor
#define LED_D1 D1   // Pin connected to LED1
#define LED_D2 D2   // Pin connected to LED2

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_D1, OUTPUT);
  pinMode(LED_D2, OUTPUT);

  Serial.println("Ultrasonic sensor test started.");
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) {
    Serial.println("No echo detected.");
    return;
  }

  float distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance < 5) {
    digitalWrite(LED_D1, LOW);
    digitalWrite(LED_D2, HIGH);
    delay(15000);
  } else {
    digitalWrite(LED_D1, HIGH);
    digitalWrite(LED_D2, LOW);
    delay(10000);

    digitalWrite(LED_D1, LOW);
    digitalWrite(LED_D2, HIGH);
    delay(10000);
  }
}
