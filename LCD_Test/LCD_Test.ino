#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);

  Serial.println("Testing LCD");
  lcd.print("Hello, world");
  Serial.println("Is the LCD on? Type anything if so");
  while (!Serial.available()){}
  Serial.println("Goodbye");
}

void loop() {
  // put your main code here, to run repeatedly:
}
