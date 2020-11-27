#pragma once

#include "ilogger.h"

/**
 * Prints debugging output to serial port
 */
class Logger : public ILogger {
  const LogLevel logLevel;

public:
  /**
   * Create logger class with log level
   *
   * @param logLevel  only messages with higher log level will be printed
   */
  Logger(LogLevel logLevel);

  void init();
  void print(LogLevel level, const char *str) override;
  void println(LogLevel level, const char *str) override;
  void print(LogLevel level, int32_t n) override;
  void println(LogLevel level) override;
};
