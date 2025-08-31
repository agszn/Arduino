// Define the pins for accelerometer outputs
#define X_PIN D5 // GPIO 14
#define Y_PIN D6 // GPIO 12
#define Z_PIN D7 // GPIO 13

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Configure pins as input
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);

  Serial.println("NodeMCU Accelerometer Test Initialized!");
}

void loop() {
  // Read the digital state of the X, Y, and Z pins
  int xState = digitalRead(X_PIN);
  int yState = digitalRead(Y_PIN);
  int zState = digitalRead(Z_PIN);

  // Print the values
  Serial.print("X: ");
  Serial.print(xState);
  Serial.print(" | Y: ");
  Serial.print(yState);
  Serial.print(" | Z: ");
  Serial.println(zState);

  // Add a delay for readability
  delay(500);
}
