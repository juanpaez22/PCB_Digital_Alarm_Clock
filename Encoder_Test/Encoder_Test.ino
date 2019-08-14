#define PIN_ENC_CLK 2       // Encoder: Generating interrupts using CLK signal
#define PIN_ENC_DT 3        // Encoder: Reading DT signal
#define PIN_ENC_SW 4        // Encoder: Reading Push Button switch

volatile boolean hasRotated;

void isr ()  {
  Serial.println("Entering isr");
  hasRotated = true;
  Serial.println("Exiting isr");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_ENC_CLK, INPUT);
  pinMode(PIN_ENC_DT, INPUT);
  pinMode(PIN_ENC_SW, INPUT);
  attachInterrupt (0,isr,FALLING);  // Interrupt 0 connected to pin 2.

  Serial.begin(115200);

  Serial.print("Press the encoder button");
  while (!(digitalRead(PIN_ENC_SW))) {}
  Serial.print("Good! Now rotate the encoder");
  hasRotated = false;
  while (!hasRotated){};
  Serial.print("Done. Goodbye");
}

void loop() {
  // put your main code here, to run repeatedly:
}
