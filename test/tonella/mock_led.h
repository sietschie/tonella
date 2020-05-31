#include "iled.h"
#include <gmock/gmock.h>

class MockLED : public ILed {
public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(void, loop, (), (override));
  MOCK_METHOD(void, start, (Mode mode, uint32_t duration, uint16_t payload),
              (override));
  MOCK_METHOD(void, set, (uint8_t r, uint8_t g, uint8_t b), (override));
};
