#pragma once

#include <stdint.h>

/**
 * interface for system abstraction
 *
 * for debugging and timestamps
 */
class ISystem {
public:
  /** initialize system class */
  virtual void init() = 0;

  /**
   * get current timestamp
   *
   * @returns timestamp in milliseconds
   */
  virtual uint32_t get_timestamp() = 0;

  /**
   * get play state
   *
   * @returns current play state, 1 means mp3 is playing
   */
  virtual uint8_t get_play_state() = 0;
};
