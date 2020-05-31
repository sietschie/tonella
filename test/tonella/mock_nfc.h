#include "infc.h"
#include <gmock/gmock.h>

class MockNFC : public INfc {
public:
  MOCK_METHOD(bool, readCard, (Type & type, uint16_t &index), (override));
  MOCK_METHOD(TagState, checkCardStatus, (Type & type, uint16_t &index),
              (override));
  MOCK_METHOD(bool, init, (), (override));
};
