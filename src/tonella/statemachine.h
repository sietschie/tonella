#pragma once

#include "iled.h"
#include "infc.h"
#include "iplayer.h"
#include "isystem.h"

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
  ISystem *system;

  void play_song(uint16_t index);
  void stop_song();
  void execute_command(uint16_t index);

  bool command_periodically_active = false;
  uint32_t command_start_time;
  uint32_t command_last_active_time;
  uint16_t command;
  void execute_command_periodically();

public:
  /**
   * initializes state machine
   *
   * @param led     led module
   * @param nfc     nfc module
   * @param player  player module
   * @param system  system module
   */
  void init(ILed *led, INfc *nfc, IPlayer *player, ISystem *system);

  /** run one iteration of the state machine */
  void run();
};
