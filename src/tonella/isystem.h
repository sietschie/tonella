#pragma once

#include <stdint.h>

/**
 * interface for system abstraction
 *
 * for debugging and timestamps
 */
class ISystem {
public:
  /**
   * get current timestamp
   *
   * @returns timestamp in milliseconds
   */
  virtual uint32_t get_timestamp() = 0;
};
