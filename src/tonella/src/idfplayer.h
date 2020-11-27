#pragma once

#include <stdint.h>

/**
 * interface to dfplayer class
 *
 * controls hardware mp3 player
 */
class IDFPlayer {
public:
  /**
   * initialize dfplayer
   *
   * @returns true if initialized succesfully
   */
  virtual bool init() = 0;

  /** pause current song */
  virtual void pause() = 0;

  /** stop current song */
  virtual void stop() = 0;

  /** resume paused song */
  virtual void resume() = 0;

  /**
   * play mp3
   *
   * @param folder  folder of mp3 to play
   * @param index   index of mp3 to play
   */
  virtual void play(uint16_t folder, uint16_t index) = 0;

  /**
   * set volume
   *
   * @param volume   volume between 0 and 30
   */
  virtual void set_volume(uint8_t volume) = 0;
};
