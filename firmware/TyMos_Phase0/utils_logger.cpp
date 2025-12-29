#include "utils_logger.h"

LoggerClass Logger;

void LoggerClass::begin() {
  Serial.begin(115200);
  // Wait for serial port? No, usually not blocking
  Serial.println("\n\n--- TyMos v2.0 Logger Initialized ---");
}

void LoggerClass::info(const char *format, ...) {
  va_list args;
  va_start(args, format);
  log("INFO", format, args);
  va_end(args);
}

void LoggerClass::warning(const char *format, ...) {
  va_list args;
  va_start(args, format);
  log("WARN", format, args);
  va_end(args);
}

void LoggerClass::error(const char *format, ...) {
  va_list args;
  va_start(args, format);
  log("ERROR", format, args);
  va_end(args);
}

void LoggerClass::getTimestamp(char *buffer, size_t bufferSize) {
  unsigned long m = millis();
  unsigned long seconds = m / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;

  snprintf(buffer, bufferSize, "[%02lu:%02lu:%02lu.%03lu]", (hours % 24),
           (minutes % 60), (seconds % 60), (m % 1000));
}

void LoggerClass::log(const char *level, const char *format, va_list args) {
  char timestamp[20];
  getTimestamp(timestamp, sizeof(timestamp));

  Serial.print(timestamp);
  Serial.print("[");
  Serial.print(level);
  Serial.print("] ");

  char msg[128];
  vsnprintf(msg, sizeof(msg), format, args);
  Serial.println(msg);
}
