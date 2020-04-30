#pragma once

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "iplayer.h"

/**
 * player class controls mp3 player module
 */
class Player : public IPlayer {
  int volume = 0;
  DFRobotDFPlayerMini myDFPlayer;
  int current_mode = 0;
  int current_track = 0;
  int mode = 0;

public:
  bool init() override;
  void pause() override;
  void play(uint16_t index) override;
  void set_volume(uint8_t volume) override;
  uint8_t get_volume() override;
  void set_mode(Mode mode) override;
  Mode get_mode() override;
};
