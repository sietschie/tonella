#include "ilogger.h"

/**
 * Discards all debug output
 */
class NullLogger : public ILogger {
public:
  void init() override;
  void print(LogLevel level, const char *str) override;
  void println(LogLevel level, const char *str) override;
  void print(LogLevel level, int32_t n) override;
  void println(LogLevel level) override;
};
