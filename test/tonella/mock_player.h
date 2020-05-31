#include "iplayer.h"
#include <gmock/gmock.h>

class MockPlayer : public IPlayer {
public:
  MOCK_METHOD(bool, init, (IDFPlayer * dfplayer, IMemory *memory), (override));
  MOCK_METHOD(void, pause, (), (override));
  MOCK_METHOD(void, stop, (), (override));
  MOCK_METHOD(void, play, (uint16_t index), (override));
  MOCK_METHOD(void, set_volume, (uint8_t volume), (override));
  MOCK_METHOD(uint8_t, get_volume, (), (override));
  MOCK_METHOD(Mode, get_mode, (), (override));
  MOCK_METHOD(void, change_to_next_mode, (), (override));
};
