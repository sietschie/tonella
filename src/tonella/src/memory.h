#include "imemory.h"

/**
 * Memory class to save and retreive data from eeprom
 */
class Memory : public IMemory {
public:
  uint8_t get_volume() override;
  void save_volume(uint8_t volume) override;
};
