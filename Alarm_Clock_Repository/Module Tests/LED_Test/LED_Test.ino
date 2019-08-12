#define PIN_ALARM_LED 6     // Alarm LED to signify alarm is enabled

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_ALARM_LED, OUTPUT);
  Serial.begin(115200);

  Serial.println("Testing LED");
  digitalWrite(PIN_ALARM_LED, HIGH);
  Serial.println("Is the LED on? Type anything if so");
  while (!Serial.available()){}
  Serial.println("Goodbye");
}

void loop() {
  // put your main code here, to run repeatedly:
}
