#include <HX711_ADC.h> // HX711 ADC library
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Make sure you're using the correct library version

HX711_ADC LoadCell(6, 7); // dt = pin 6, sck = pin 7
LiquidCrystal_I2C lcd(0x27, 16, 2); // correct constructor for most newer LiquidCrystal_I2C libraries

void setup() {
  LoadCell.begin();
  LoadCell.start(2000); // 2 second startup delay
  LoadCell.setCalFactor(1000.0); // adjust to calibrate
  lcd.begin(); // no arguments required in many library versions
  lcd.backlight();
}

void loop() {
  LoadCell.update();
  float i = LoadCell.getData();

  lcd.setCursor(0, 0);
  lcd.print("Weight[g]:     "); // padded to clear old values
  lcd.setCursor(0, 1);
  lcd.print(i, 2); // show 2 decimal places
}
