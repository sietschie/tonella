#pragma once

#include "iled.h"
#include "infc.h"
#include "iplayer.h"

#include <stdint.h>

/**
 * the tonella state machine
 */
class StateMachine {
  uint8_t state = IDLE;
  enum State_enum { IDLE, PLAYING, COMMAND };
  enum Command_enum {
    COMMAND_VOLUME_DOWN_ID = 1,
    COMMAND_VOLUME_UP_ID = 2,
    COMMAND_MODE_CHANGE = 3
  };

  ILed *led;
  INfc *nfc;
  IPlayer *player;

  void play_song(int index);
  void stop_song();
  void execute_command(int index);

public:
  /**
   * initializes state machine
   *
   * @param led     led module
   * @param nfc     nfc module
   * @param player  player module
   */
  void init(ILed *led, INfc *nfc, IPlayer *player);

  /** run one iteration of the state machine */
  void run();
};
