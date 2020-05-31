#include "player.cpp"
#include "statemachine.cpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockLED : public ILed {
public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(void, loop, (), (override));
  MOCK_METHOD(void, start, (Mode mode, uint32_t duration, uint16_t payload),
              (override));
  MOCK_METHOD(void, set, (uint8_t r, uint8_t g, uint8_t b), (override));
};

class MockNFC : public INfc {
public:
  MOCK_METHOD(bool, readCard, (Type & type, uint16_t &index), (override));
  MOCK_METHOD(TagState, checkCardStatus, (Type & type, uint16_t &index),
              (override));
  MOCK_METHOD(bool, init, (), (override));
};

class MockDFPlayer : public IDFPlayer {
public:
  MOCK_METHOD(bool, init, (), (override));
  MOCK_METHOD(void, pause, (), (override));
  MOCK_METHOD(void, resume, (), (override));
  MOCK_METHOD(void, play, (uint16_t folder, uint16_t index), (override));
  MOCK_METHOD(void, set_volume, (uint8_t volume), (override));
};

class MockMemory : public IMemory {
public:
  MOCK_METHOD(uint8_t, get_volume, (), (override));
  MOCK_METHOD(void, save_volume, (uint8_t volume), (override));
};

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

class MockSystem : public ISystem {
public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(uint32_t, get_timestamp, (), (override));
  MOCK_METHOD(uint8_t, get_play_state, (), (override));
};

class MockLogger : public ILogger {
public:
  MOCK_METHOD(void, init, (), (override));
  MOCK_METHOD(void, print, (LogLevel level, const char *str), (override));
  MOCK_METHOD(void, println, (LogLevel level, const char *str), (override));
  MOCK_METHOD(void, print, (LogLevel level, int32_t n), (override));
  MOCK_METHOD(void, println, (LogLevel level), (override));
};

using ::testing::_;
using ::testing::Ge;
using ::testing::Le;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgReferee;

TEST(StateMachine, PlaySong) {
  NiceMock<MockLED> led;
  MockNFC nfc;
  MockPlayer player;
  MockLogger logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::FIGURINE),
                      SetArgReferee<1>(1), Return(INfc::TagState::TAG_FOUND)));
  EXPECT_CALL(player, play(1));
  ON_CALL(led, start(_, _, _)).WillByDefault(Return());

  state_machine.run();
}

TEST(StateMachine, StopSong) {
  NiceMock<MockLED> led;
  MockNFC nfc;
  MockPlayer player;
  MockLogger logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::FIGURINE),
                      SetArgReferee<1>(1), Return(INfc::TagState::TAG_FOUND)))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::FIGURINE),
                      SetArgReferee<1>(1), Return(INfc::TagState::TAG_GONE)));
  EXPECT_CALL(player, play(1));
  EXPECT_CALL(player, pause());
  ON_CALL(led, start(_, _, _)).WillByDefault(Return());

  state_machine.run();
  state_machine.run();
}

TEST(StateMachine, IncreaseVolume) {
  MockLED led;
  MockNFC nfc;
  MockPlayer player;
  MockLogger logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::COMMAND),
                      SetArgReferee<1>(2), Return(INfc::TagState::TAG_FOUND)));
  EXPECT_CALL(player, set_volume(Ge(1)));
  EXPECT_CALL(led, start(_, _, _));
  EXPECT_CALL(player, get_volume()).WillRepeatedly(Return(0));

  state_machine.run();
}

TEST(StateMachine, DecreaseVolume) {
  MockLED led;
  MockNFC nfc;
  MockPlayer player;
  MockLogger logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::COMMAND),
                      SetArgReferee<1>(1), Return(INfc::TagState::TAG_FOUND)));
  EXPECT_CALL(player, set_volume(Le(29)));
  EXPECT_CALL(led, start(_, _, _));
  EXPECT_CALL(player, get_volume()).WillRepeatedly(Return(30));

  state_machine.run();
}

TEST(StateMachine, ChangeMode) {
  MockLED led;
  MockNFC nfc;
  MockPlayer player;
  MockLogger logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::COMMAND),
                      SetArgReferee<1>(3), Return(INfc::TagState::TAG_FOUND)));
  EXPECT_CALL(player, change_to_next_mode());

  state_machine.run();
}

TEST(Player, SetVolume) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);

  EXPECT_CALL(dfplayer, set_volume(20));
  EXPECT_CALL(memory, save_volume(20));
  player.set_volume(20);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
