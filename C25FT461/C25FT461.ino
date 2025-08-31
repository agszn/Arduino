#define TRIG_PIN D5
#define ECHO_PIN D6
#define BUZZER_PIN D7

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Send 10us pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read pulse duration with timeout
  duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  if (duration == 0) {
    distance = 999;
    Serial.println("Out of range or sensor error");
  } else {
    distance = duration * 0.034 / 2;
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Buzzer logic
  if (distance < 20) {
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    digitalWrite(BUZZER_PIN, HIGH]]);
  }

  delay(500);
}
