#pragma once

#include "idfplayer.h"
#include "imemory.h"

/**
 * interface to player class
 *
 * controls mp3 player module
 */
class IPlayer {
public:
  /**
   * different player modes
   *
   * Sound   playes a sound and says the name of the figurine
   * Song    plays a song associated with the figurine
   */
  enum Mode { Sound, Song };

  /**
   * initialize player
   *
   * @param dfplayer  module to access dfplayer functions
   * @param memory    module to access memory functions
   *
   * @returns true if initialized succesfully
   */
  virtual bool init(IDFPlayer *dfplayer, IMemory *memory) = 0;

  /** pause current song */
  virtual void pause() = 0;

  /** stop current song */
  virtual void stop() = 0;
  virtual void play_beeps() = 0;

  /**
   * play song according to current mode
   *
   * @param index   index of song to play
   */
  virtual void play(uint16_t index) = 0;

  /**
   * set volume
   *
   * @param volume   volume between 0 and 30
   */
  virtual void set_volume(uint8_t volume) = 0;

  /**
   * get current volume
   *
   * @returns current volume
   */
  virtual uint8_t get_volume() = 0;

  /**
   * return current player mode
   *
   * @returns current player mode
   */
  virtual Mode get_mode() = 0;

  /** switches to next mode in rotating manner */
  virtual void change_to_next_mode() = 0;
};
