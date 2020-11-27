#pragma once

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "idfplayer.h"

/**
 * player class controls dfplayer module
 */
class DFPlayer : public IDFPlayer {
  DFRobotDFPlayerMini myDFPlayer;

public:
  bool init() override;
  void pause() override;
  void stop() override;
  void resume() override;
  void play(uint16_t folder, uint16_t index) override;
  void set_volume(uint8_t volume) override;
};
