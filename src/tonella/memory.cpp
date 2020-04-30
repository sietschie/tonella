#include "memory.h"

#include <EEPROM.h>

byte Memory::get_volume() {
  byte volume = EEPROM.read(0);
  if (volume > 30)
    volume = 30;

  return volume;
}

void Memory::save_volume(byte volume) { EEPROM.update(0, volume); }
