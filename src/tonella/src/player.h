#pragma once

#include "iplayer.h"

/**
 * player class controls mp3 player module
 */
class Player : public IPlayer {
  int volume = 0;
  IDFPlayer *dfplayer;
  IMemory *memory;
  int current_mode = 0;
  int current_track = 0;
  Mode mode = Song;

public:
  bool init(IDFPlayer *dfplayer, IMemory *memory) override;
  void pause() override;
  void stop() override;
  void play(uint16_t index) override;
  void play_beeps();
  void set_volume(uint8_t volume) override;
  uint8_t get_volume() override;
  Mode get_mode() override;
  void change_to_next_mode() override;
};
