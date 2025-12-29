#ifndef HW_RTC_H
#define HW_RTC_H

#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>

class RTCDriver {
public:
  RTCDriver();
  bool begin();
  DateTime now();
  void setTime(DateTime dt);
  void syncWithCompileTime();
  float getTemperature();

private:
  RTC_DS3231 _rtc;
  DateTime _getCompileDateTime();
};

extern RTCDriver RTC;

#endif // HW_RTC_H
