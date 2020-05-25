#pragma once

#include <stdint.h>

/**
 * Interface to memory class
 *
 * Using this class persistent information like volume
 * can be safed and retreived.
 */
class IMemory {
public:
  /**
   * return saved volume
   *
   * @returns saved volume
   */
  virtual uint8_t get_volume() = 0;

  /**
   * save volume to memory
   *
   * @param volume   the volume to be saved
   */
  virtual void save_volume(uint8_t volume) = 0;
};
