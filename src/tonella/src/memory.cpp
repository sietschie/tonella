#include "memory.h"

#include <EEPROM.h>

uint8_t Memory::get_volume() {
  uint8_t volume = EEPROM.read(0);
  if (volume > 30)
    volume = 30;

  return volume;
}

void Memory::save_volume(uint8_t volume) { EEPROM.update(0, volume); }
