#pragma once

#include "Arduino.h"
#include "isystem.h"

/**
 * system abstraction for debugging and timestamps
 */
class System : public ISystem {
public:
  void init() override;
  uint32_t get_timestamp() override;
  uint8_t get_play_state() override;
};
