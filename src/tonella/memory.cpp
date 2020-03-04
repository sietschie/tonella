#include "memory.h"

#include <EEPROM.h>

byte memory_get_volume() {
  byte volume = EEPROM.read(0);
  if (volume > 30)
    volume = 30;

  return volume;
}

void memory_save_volume(byte volume) { EEPROM.update(0, volume); }
