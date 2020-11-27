#include "logger.h"
#include "Arduino.h"

Logger::Logger(LogLevel logLevel) : logLevel(logLevel) {}

void Logger::init() { Serial.begin(115200); }

void Logger::print(LogLevel level, const char *str) {
  if (level <= logLevel)
    Serial.print(str);
}

void Logger::println(LogLevel level, const char *str) {
  if (level <= logLevel)
    Serial.println(str);
}

void Logger::print(LogLevel level, int32_t n) {
  if (level <= logLevel)
    Serial.print(n);
}

void Logger::println(LogLevel level) {
  if (level <= logLevel)
    Serial.println();
}
