#include "idfplayer.h"
#include <gmock/gmock.h>

class MockDFPlayer : public IDFPlayer {
public:
  MOCK_METHOD(bool, init, (), (override));
  MOCK_METHOD(void, pause, (), (override));
  MOCK_METHOD(void, stop, (), (override));
  MOCK_METHOD(void, resume, (), (override));
  MOCK_METHOD(void, play, (uint16_t folder, uint16_t index), (override));
  MOCK_METHOD(void, set_volume, (uint8_t volume), (override));
};
