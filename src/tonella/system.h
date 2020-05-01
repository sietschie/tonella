#pragma once

#include "Arduino.h"
#include "isystem.h"

/**
 * system abstraction for debugging and timestamps
 */
class System : public ISystem {
public:
  uint32_t get_timestamp();
};
