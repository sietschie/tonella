
#include "player.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mock_dfplayer.h"
#include "mock_memory.h"

using ::testing::_;
using ::testing::Ge;
using ::testing::Le;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Sequence;
using ::testing::SetArgReferee;

TEST(Player, SetVolume) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);

  EXPECT_CALL(dfplayer, set_volume(20));
  EXPECT_CALL(memory, save_volume(20));
  player.set_volume(20);
}

TEST(Player, GetVolume) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);

  player.set_volume(20);
  EXPECT_EQ(player.get_volume(), 20);
}

TEST(Player, DefaultModeIsSong) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);

  EXPECT_EQ(player.get_mode(), IPlayer::Song);
}

TEST(Player, ModeChange) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);
  player.change_to_next_mode();

  EXPECT_NE(player.get_mode(), IPlayer::Song);
}

TEST(Player, ModeChangeTwice) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);
  player.change_to_next_mode();
  player.change_to_next_mode();

  EXPECT_EQ(player.get_mode(), IPlayer::Song);
}

TEST(Player, PlaySong) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);

  EXPECT_CALL(dfplayer, play(2, 10));
  player.play(10);
}

TEST(Player, PlaySound) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);
  player.change_to_next_mode();

  EXPECT_CALL(dfplayer, play(3, 10));
  player.play(10);
}

TEST(Player, Pause) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);

  EXPECT_CALL(dfplayer, pause());
  player.pause();
}

TEST(Player, Stop) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);

  EXPECT_CALL(dfplayer, pause());
  player.stop();
}

TEST(Player, ResumeWhenPaused) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);

  Sequence s;

  EXPECT_CALL(dfplayer, play(2, 10)).InSequence(s);
  EXPECT_CALL(dfplayer, pause()).InSequence(s);
  EXPECT_CALL(dfplayer, resume()).InSequence(s);
  player.play(10);
  player.pause();
  player.play(10);
}

TEST(Player, DontResumeWhenTrackChanged) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);

  Sequence s;

  EXPECT_CALL(dfplayer, play(2, 10)).InSequence(s);
  EXPECT_CALL(dfplayer, pause()).InSequence(s);
  EXPECT_CALL(dfplayer, play(2, 11)).InSequence(s);
  player.play(10);
  player.pause();
  player.play(11);
}

TEST(Player, DontResumeWhenStopped) {
  NiceMock<MockDFPlayer> dfplayer;
  NiceMock<MockMemory> memory;

  Player player;
  player.init(&dfplayer, &memory);

  Sequence s;

  EXPECT_CALL(dfplayer, play(2, 10)).InSequence(s);
  EXPECT_CALL(dfplayer, pause()).InSequence(s);
  EXPECT_CALL(dfplayer, play(2, 10)).InSequence(s);
  player.play(10);
  player.stop();
  player.play(10);
}
