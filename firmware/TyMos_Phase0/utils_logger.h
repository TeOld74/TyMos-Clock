#ifndef UTILS_LOGGER_HPP
#define UTILS_LOGGER_HPP

#include <Arduino.h>

// ============================================================================
// LOGGER - Sistema logging multilivello
// ============================================================================

class LoggerClass {
public:
  void begin();
  void info(const char* format, ...);
  void warning(const char* format, ...);
  void error(const char* format, ...);

private:
  void log(const char* level, const char* format, va_list args);
  void getTimestamp(char* buffer, size_t bufferSize);
};

extern LoggerClass Logger;

#endif // UTILS_LOGGER_HPP
