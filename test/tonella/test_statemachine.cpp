
#include "statemachine.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mock_dfplayer.h"
#include "mock_led.h"
#include "mock_logger.h"
#include "mock_memory.h"
#include "mock_nfc.h"
#include "mock_player.h"
#include "mock_system.h"

using ::testing::_;
using ::testing::Ge;
using ::testing::Le;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Sequence;
using ::testing::SetArgReferee;
using ::testing::DoAll;

TEST(StateMachine, PlaySong) {
  NiceMock<MockLED> led;
  MockNFC nfc;
  MockPlayer player;
  NiceMock<MockLogger> logger;
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
  NiceMock<MockLogger> logger;
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
  NiceMock<MockLogger> logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::COMMAND),
                      SetArgReferee<1>(2), Return(INfc::TagState::TAG_FOUND)));
  EXPECT_CALL(player, set_volume(Ge(1)));
  EXPECT_CALL(player, play_beeps());
  EXPECT_CALL(led, start(_, _, _));
  EXPECT_CALL(player, get_volume()).WillRepeatedly(Return(0));

  state_machine.run();
}

TEST(StateMachine, DecreaseVolume) {
  MockLED led;
  MockNFC nfc;
  MockPlayer player;
  NiceMock<MockLogger> logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::COMMAND),
                      SetArgReferee<1>(1), Return(INfc::TagState::TAG_FOUND)));
  EXPECT_CALL(player, set_volume(Le(29)));
  EXPECT_CALL(player, play_beeps());
  EXPECT_CALL(led, start(_, _, _));
  EXPECT_CALL(player, get_volume()).WillRepeatedly(Return(30));

  state_machine.run();
}

TEST(StateMachine, MaxVolume) {
  MockLED led;
  MockNFC nfc;
  MockPlayer player;
  NiceMock<MockLogger> logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::COMMAND),
                      SetArgReferee<1>(2), Return(INfc::TagState::TAG_FOUND)));
  EXPECT_CALL(player, set_volume(_)).Times(0);
  EXPECT_CALL(player, get_volume()).WillRepeatedly(Return(30));
  EXPECT_CALL(player, stop());

  state_machine.run();
}

TEST(StateMachine, MinVolume) {
  MockLED led;
  MockNFC nfc;
  MockPlayer player;
  NiceMock<MockLogger> logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::COMMAND),
                      SetArgReferee<1>(1), Return(INfc::TagState::TAG_FOUND)));
  EXPECT_CALL(player, set_volume(_)).Times(0);
  EXPECT_CALL(player, get_volume()).WillRepeatedly(Return(7));
  EXPECT_CALL(player, stop());

  state_machine.run();
}

TEST(StateMachine, PeriodicCommand) {
  NiceMock<MockLED> led;
  MockNFC nfc;
  NiceMock<MockPlayer> player;
  NiceMock<MockLogger> logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::COMMAND),
                      SetArgReferee<1>(1), Return(INfc::TagState::TAG_FOUND)))
      .WillOnce(Return(INfc::TagState::TAG_UNCHANGED))
      .WillOnce(Return(INfc::TagState::TAG_UNCHANGED))
      .WillOnce(Return(INfc::TagState::TAG_GONE));

  Sequence s;
  EXPECT_CALL(player, set_volume(Le(29))).InSequence(s);
  EXPECT_CALL(player, set_volume(Le(28))).InSequence(s);
  EXPECT_CALL(player, set_volume(Le(27))).InSequence(s);

  EXPECT_CALL(player, get_volume())
      .WillOnce(Return(30))
      .WillOnce(Return(29))
      .WillOnce(Return(28));

  EXPECT_CALL(system, get_timestamp())
      .WillOnce(Return(0))
      .WillOnce(Return(1010))
      .WillOnce(Return(1520));

  state_machine.run();
  state_machine.run();
  state_machine.run();
  state_machine.run();
}

TEST(StateMachine, ChangeMode) {
  NiceMock<MockLED> led;
  MockNFC nfc;
  MockPlayer player;
  NiceMock<MockLogger> logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::COMMAND),
                      SetArgReferee<1>(3), Return(INfc::TagState::TAG_FOUND)));
  EXPECT_CALL(player, change_to_next_mode());

  state_machine.run();
}

TEST(StateMachine, FinishSong) {
  NiceMock<MockLED> led;
  MockNFC nfc;
  MockPlayer player;
  NiceMock<MockLogger> logger;
  NiceMock<MockSystem> system;

  StateMachine state_machine;
  state_machine.init(&led, &nfc, &player, &system, &logger);

  EXPECT_CALL(nfc, checkCardStatus(_, _))
      .WillOnce(DoAll(SetArgReferee<0>(INfc::Type::FIGURINE),
                      SetArgReferee<1>(1), Return(INfc::TagState::TAG_FOUND)))
      .WillOnce(Return(INfc::TagState::TAG_UNCHANGED))
      .WillOnce(Return(INfc::TagState::TAG_GONE));
  EXPECT_CALL(player, play(1));
  EXPECT_CALL(player, pause());
  EXPECT_CALL(system, get_play_state())
      .WillOnce(Return(1))
      .WillOnce(Return(0))
      .WillOnce(Return(0));
  ON_CALL(led, start(_, _, _)).WillByDefault(Return());

  Sequence s;
  EXPECT_CALL(led, set(0, 1, 0)).InSequence(s);
  EXPECT_CALL(led, set(1, 1, 0)).InSequence(s);
  EXPECT_CALL(led, set(1, 0, 0)).InSequence(s);

  state_machine.run();
  state_machine.run();
  state_machine.run();
}
