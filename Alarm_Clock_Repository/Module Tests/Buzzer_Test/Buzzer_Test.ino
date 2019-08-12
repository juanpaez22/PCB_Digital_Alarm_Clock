#define PIN_BUZZER 15

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_BUZZER, OUTPUT);
  Serial.begin(115200);

  Serial.print("Playing alarm. Type anything to stop");
  while (!Serial.available()) {
      Serial.println("Alarm!");
      digitalWrite(PIN_BUZZER, HIGH);
      delay(50);
      digitalWrite(PIN_BUZZER, LOW);
      delay(50);
    }
  Serial.print("Done. Goodbye");
}

void loop() {
  // put your main code here, to run repeatedly:
}
