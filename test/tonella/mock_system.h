#include "isystem.h"
#include <gmock/gmock.h>

class MockSystem : public ISystem {
public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(uint32_t, get_timestamp, (), (override));
  MOCK_METHOD(uint8_t, get_play_state, (), (override));
};
