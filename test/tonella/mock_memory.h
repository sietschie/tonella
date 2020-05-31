#include "imemory.h"
#include <gmock/gmock.h>

class MockMemory : public IMemory {
public:
  MOCK_METHOD(uint8_t, get_volume, (), (override));
  MOCK_METHOD(void, save_volume, (uint8_t volume), (override));
};
