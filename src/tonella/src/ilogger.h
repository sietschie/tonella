#pragma once

#include <stdint.h>

/**
 * Interface to logging class
 *
 * Prints debugging output to serial port
 */
class ILogger {
public:
  /** available log levels */
  enum LogLevel { Error, Warn, Info, Debug };

  /** init logger */
  virtual void init() = 0;

  /**
   * print string
   *
   * @param level  log level for message
   * @param str    string to be printed
   */
  virtual void print(LogLevel level, const char *str) = 0;

  /**
   * print string with newline
   *
   * @param level  log level for message
   * @param str    string to be printed
   */
  virtual void println(LogLevel level, const char *str) = 0;

  /**
   * print number
   *
   * @param level  log level for message
   * @param n      number to be printed
   */
  virtual void print(LogLevel level, int32_t n) = 0;

  /**
   * print newline
   *
   * @param level  log level for message
   */
  virtual void println(LogLevel level) = 0;
};
