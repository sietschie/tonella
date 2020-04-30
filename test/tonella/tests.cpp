#include "statemachine.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockLED : public ILed {
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(void, loop, (), (override));
  MOCK_METHOD(void, start, (Mode mode, uint32_t duration, uint16_t payload), (override));
  MOCK_METHOD(void, set, (uint8_t r, uint8_t g, uint8_t b), (override));
};

class MockNFC : public INfc {};
class MockPlayer : public IPlayer {};

TEST(Test1, Test11) {
  MockLED led;

  StateMachine state_machine();
  ASSERT_EQ(1, 1);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
