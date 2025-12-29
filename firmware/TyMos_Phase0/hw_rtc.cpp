#include "hw_rtc.h"
#include "utils_logger.h"

RTCDriver RTC;

RTCDriver::RTCDriver() {}

bool RTCDriver::begin() {
  if (!_rtc.begin()) {
    Logger.error("Couldn't find RTC");
    return false;
  }

  if (_rtc.lostPower()) {
    Logger.warning("RTC lost power, setting to compile time!");
    _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Note: syncWithCompileTime() is now called only as fallback if NTP fails
  // The main sketch handles NTP sync first, then falls back to compile time

  return true;
}

DateTime RTCDriver::now() { return _rtc.now(); }

void RTCDriver::setTime(DateTime dt) {
  _rtc.adjust(dt);
  Logger.info("RTC time manually set");
}

float RTCDriver::getTemperature() { return _rtc.getTemperature(); }

void RTCDriver::syncWithCompileTime() {
  DateTime compiled = DateTime(F(__DATE__), F(__TIME__));

  // ALWAYS overwrite RTC with compile time (as per user requirement)
  _rtc.adjust(compiled);
  Logger.info("RTC sincronizzato con ora compilazione: %02d:%02d:%02d",
              compiled.hour(), compiled.minute(), compiled.second());
}
