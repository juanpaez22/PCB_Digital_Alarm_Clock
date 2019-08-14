#define PIN_BUZZER 15

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_BUZZER, OUTPUT);
  Serial.begin(115200);

  Serial.print("Playing alarm. Type anything to stop");
  while (!Serial.available()) {
      Serial.println("Alarm!");
      tone(PIN_BUZZER, 1000, 250);
      delay(500);
    }
  Serial.print("Done. Goodbye");
}

void loop() {
  // put your main code here, to run repeatedly:
}
