#include <RtcDS1307.h>
#include <Wire.h>

RtcDS1307<TwoWire> Rtc(Wire);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
  Rtc.SetDateTime(compiled);

  Serial.print("Testing RTC. Time should display repeatedly below. Type anything to exit");
  while (!Serial.available()){
    Serial.print(Rtc.GetDateTime());
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
