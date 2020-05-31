#include "ilogger.h"
#include <gmock/gmock.h>

class MockLogger : public ILogger {
public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(void, print, (LogLevel level, const char *str), (override));
  MOCK_METHOD(void, println, (LogLevel level, const char *str), (override));
  MOCK_METHOD(void, print, (LogLevel level, int32_t n), (override));
  MOCK_METHOD(void, println, (LogLevel level), (override));
};
