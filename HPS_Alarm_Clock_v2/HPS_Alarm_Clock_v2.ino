// PCB digital alarm clock code for Google Hardware Product Sprint program, Summer 2019.
// Consists of an Arduino-based MCU interfaced to an I2C real-time clock, LCD display,
// rotary encoder input, alarm-state LED, passive buzzer, and power supply. For block diagram,
// schematic, and PCB layout, see link below:
//
// https://upverter.com/design/hardwareproductsprint/347b8ea7292525cc/paez_juan_clock_submitted/

#include <LiquidCrystal.h>
#include <RtcDS1307.h>
#include <Wire.h>

#define PIN_ALARM_LED 6     // Alarm LED to signify alarm is enabled
#define PIN_ENC_CLK 2       // Encoder: Generating interrupts using CLK signal
#define PIN_ENC_DT 3        // Encoder: Reading DT signal
#define PIN_ENC_SW 4        // Encoder: Reading Push Button switch
#define PIN_BUZZER 15       // Passive buzzer


//////////////////////////////////////// GLOBAL VARIABLES /////////////////////////////////////////////////

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);   // LCD object.
volatile boolean turn_detected;           // Set to true if encoder is turned, need volatile for interrupts.
volatile boolean rotation_direction;      // CW or CCW rotation of encoder.
boolean alarm_on = false;                 // Alarm state.
int alarm_hour = 0;                       // Alarm hour set.
int alarm_minute = 0;                     // Alarm minute set.
RtcDS1307<TwoWire> Rtc(Wire);             // I2C RTC object.
RtcDateTime previous_time;                // Store previous time to compare in main loop.


//////////////////////////////////////// FUNCTION IMPLEMENTATIONS //////////////////////////////////////////

// Interrupt routine that runs if the encoder CLK goes from HIGH to LOW.
// This sets rotation direction and sets turn detected to true.
void isr ()  {
  delay(4);  // delay for Debouncing
  if (digitalRead(PIN_ENC_CLK))
    rotation_direction = !digitalRead(PIN_ENC_DT);
  else
    rotation_direction = digitalRead(PIN_ENC_DT);
  turn_detected = true;
}

// Prompt user to set alarm hour and minute using rotary encoder and built-in encoder switch.
void setAlarm(){
  while(!digitalRead(PIN_ENC_SW)){};  // Hold until switch released.

  // Set hour.
  int set_hour = 12;
  lcd.clear();
  lcd.print("Set hour:");
  lcd.setCursor(0,1);
  lcd.print("12:00 PM");
  int print_hour = set_hour;
  boolean pm = false;

  // Hour set logic, loop until switch is pressed.
  while (digitalRead(PIN_ENC_SW)){
    if (turn_detected){
      if (rotation_direction) {
        set_hour = (set_hour + 1) % 24;
      } else{
        set_hour = set_hour - 1;
        if (set_hour < 0) set_hour = 23;
      }
      lcd.clear();
      lcd.print("Set hour:");
      lcd.setCursor(0,1);
      pm = false;
      print_hour = set_hour;
      if (print_hour == 12){
        pm = true;
      } else if (print_hour == 0) {
          print_hour = 12;
      } else if (print_hour > 12) {
          print_hour -= 12;
          pm = true;
      }
      lcd.print(print_hour, DEC);
      lcd.print(":00");
      if (pm) lcd.print(" PM");
      else    lcd.print(" AM");
      turn_detected = false;
    }
  }
  alarm_hour = set_hour;

  while(!digitalRead(PIN_ENC_SW)){};    // Hold until switch released.

  // Set minute.
  int set_minute = 0;
  lcd.clear();
  lcd.print("Set minute:");
  lcd.setCursor(0, 1);
  lcd.print(print_hour, DEC);
  lcd.print(":00");
  if (pm) lcd.print(" PM");
  else    lcd.print(" AM");

  // Minute set logic, loop until switch is pressed.
  while (digitalRead(PIN_ENC_SW)){      // Hold until switch pressed.
    if (turn_detected){
      if (rotation_direction){
        set_minute = (set_minute + 1) % 60;
      } else{
        set_minute = set_minute - 1;
        if (set_minute < 0) set_minute = 59;
      }
      lcd.clear();
      lcd.print("Set minute:");
      lcd.setCursor(0,1);
      lcd.print(print_hour, DEC);
      lcd.print(":");
      if (set_minute < 10) lcd.print("0");
      lcd.print(set_minute, DEC);
      if (pm) lcd.print(" PM");
      else    lcd.print(" AM");
      turn_detected = false;
    }
  }

  // Set alarm info.
  alarm_hour = set_hour;
  alarm_minute = set_minute;
  alarm_on = true;
  digitalWrite(PIN_ALARM_LED, HIGH);

  while (!digitalRead(PIN_ENC_SW)){};
}

// Sound alarm until user presses switch and reset alarm information.
void triggerAlarm(){
  lcd.clear();
  lcd.print("!!! ALARM !!!");
  while (digitalRead(PIN_ENC_SW)) {
    tone(PIN_BUZZER, 1000, 250);
    delay(250);
    digitalWrite(PIN_ALARM_LED, LOW);
    delay(250);
    digitalWrite(PIN_ALARM_LED, HIGH);
  }
  digitalWrite(PIN_ALARM_LED, LOW);
  alarm_on = false;
  alarm_hour = 0;
  alarm_minute = 0;
  while (!digitalRead(PIN_ENC_SW)) {}
}

// Given an int representing the day of week from the RTC date time, this function
// returns a string denoting the corresponding day of the week.
const char* DayOfWeekString(int DoW) {
  switch (DoW) {
  case 0:
    return "Sun";
  case 1:
    return "Mon";
  case 2:
    return "Tue";
  case 3:
    return "Wed";
  case 4:
    return "Thu";
  case 5:
    return "Fri";
  case 6:
    return "Sat";
  default:
    return "---";
  }
}

// Given a time from the RTC object, this function will print out the date and time in
// a legible format to the LCD.
void LcdTimePrint (const RtcDateTime& time) {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  uint8_t hour = time.Hour();
  
  // Handle 12-hour AM/PM conversions.
  boolean pm = false;
  if (hour == 12){
    pm = true;
  } else if (hour == 0) {
    hour = 12;
  } else if (hour > 12) {
    hour -= 12;
    pm = true;
  }
  
  uint8_t minute = time.Minute();
  uint8_t second = time.Second();

  // Print date.
  lcd.print(DayOfWeekString(time.DayOfWeek()));
  lcd.print(" ");
  lcd.print(time.Month(), DEC);
  lcd.print("-");
  lcd.print(time.Day(), DEC);
  lcd.print("-");
  lcd.print(time.Year(), DEC);

  // Print time.
  lcd.setCursor(0, 1);
  lcd.print(hour, DEC);
  lcd.print(":");
  if (minute < 10) { lcd.print("0"); }
  lcd.print(minute, DEC);
  lcd.print(":");
  if (second < 10) { lcd.print("0"); }
  lcd.print(second, DEC);
  if (pm) {
      lcd.print(" PM");
  } else {
      lcd.print(" AM");
  }
}


////////////////////////////////////////////////// SETUP ////////////////////////////////////////////////////

// Set RTC to compile time, pin modes, begin object listening, set up interrupt etc.
void setup() {
  pinMode(PIN_ALARM_LED, OUTPUT);
  pinMode(PIN_ENC_CLK, INPUT);
  pinMode(PIN_ENC_DT, INPUT);
  pinMode(PIN_ENC_SW, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_ENC_SW, HIGH);   // Pull-Up resistor for encoder switch.
  attachInterrupt (0,isr,FALLING);  // Interrupt 0 connected to pin 2.
  
  Serial.begin(115200);
  lcd.begin(16, 2);
  Wire.begin();
  Rtc.Begin();
  
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  // Set time if not running or not valid.
  if (!Rtc.IsDateTimeValid()) {
    Rtc.SetDateTime(compiled);
  }
  if (!Rtc.GetIsRunning()) {
    Rtc.SetIsRunning(true);
  }

  // Compare with compile time to avoid resetting time on power loss.
  RtcDateTime now = Rtc.GetDateTime();
  if (uint32_t(now) < uint32_t(compiled)) {
    Rtc.SetDateTime(compiled);
  }

  // UNCOMMENT BELOW TO SET DATE AND TIME UNCONDITIONALLY.
  // Rtc.SetDateTime(compiled);

  Rtc.SetSquareWavePin(DS1307SquareWaveOut_Low);
  previous_time = Rtc.GetDateTime();
}


//////////////////////////////////////////////// MAIN LOOP /////////////////////////////////////////////////

void loop() {
  const RtcDateTime now = Rtc.GetDateTime();
  // Check if alarm needs to be set, if alarm needs to go off, or if time needs to be updated.
  if (!(digitalRead(PIN_ENC_SW))) {
    setAlarm();
  } else if (alarm_on && now.Hour() == alarm_hour && now.Minute() == alarm_minute) {
    triggerAlarm();
  } else if (now.Second() != previous_time.Second()) {
    previous_time = now;
    LcdTimePrint(now);
  }
}
