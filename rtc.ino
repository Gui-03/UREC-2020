#include <Wire.h>
#include <RTClib.h>
#include "RF.h"

#define ALARM_PIN 2

RTC_DS3231 rtc;
void updateRTC();
void print_date();

void setup() {
  Serial.begin(9600);
  rtc.begin();  
  pinMode(ALARM_PIN, INPUT_PULLUP);
  rtc.disableAlarm(1);
  rtc.clearAlarm(1);
  rtc.writeSqwPinMode(DS3231_OFF);
  rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 0, 10), DS3231_A1_Second);
}

void loop() {
if (digitalRead(ALARM_PIN) == LOW) {
    Serial.println("Alarm at:");
    print_date();
    
    // Disable and clear alarm
    rtc.disableAlarm(1);
    rtc.clearAlarm(1);

    // Perhaps reset to new time if required
    rtc.setAlarm1(rtc.now() + TimeSpan(0, 0, 0, 10), DS3231_A1_Second); // Set for another 10 seconds
  }
  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'u') updateRTC();  // update RTC time
  }

}

void updateRTC()
{
  const char txt[6][15] = { "year [4-digit]", "month [1~12]", "day [1~31]",
                            "hours [0~23]", "minutes [0~59]", "seconds [0~59]"};
  String str = "";
  long newDate[6];

  while (Serial.available()) {
    Serial.read();
  }
  for (int i = 0; i < 6; i++) {

    Serial.print("Enter ");
    Serial.print(txt[i]);
    Serial.print(": ");

    while (!Serial.available()) {
      ;
    }
    str = Serial.readString();
    newDate[i] = str.toInt();
    Serial.println(newDate[i]);
  }

  // update RTC
  rtc.adjust(DateTime(newDate[0], newDate[1], newDate[2], newDate[3], newDate[4], newDate[5]));
  Serial.println("RTC Updated!");
}

void print_date()
{
  const char dayInWords[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

  const char monthInWords[13][4] = {" ", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
                                         "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

  DateTime rtcTime = rtc.now();

  int ss = rtcTime.second();
  int mm = rtcTime.minute();
  int hh = rtcTime.twelveHour();
  int DD = rtcTime.dayOfTheWeek();
  int dd = rtcTime.day();

  int MM = rtcTime.month();
  int yyyy = rtcTime.year();
  if (dd < 10) Serial.print("0");
  Serial.print(dd);
  Serial.print("-");
  Serial.print(monthInWords[MM]);
  Serial.print("-");
  Serial.print(yyyy);

  Serial.print("  ");
  Serial.print(dayInWords[DD]);
  Serial.print("  ");


  if (hh < 10) Serial.print("0");
  Serial.print(hh);
  Serial.print(':');

  if (mm < 10) Serial.print("0");
  Serial.print(mm);
  Serial.print(':');

  if (ss < 10) Serial.print("0");
  Serial.print(ss);

  if (rtcTime.isPM()) Serial.println(" PM");
  else Serial.println(" AM");
}
