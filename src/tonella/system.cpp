#include "system.h"

#define BUSY_PIN 4

void System::init() { pinMode(BUSY_PIN, INPUT); }

uint32_t System::get_timestamp() { return millis(); }

uint8_t System::get_play_state() {
  uint8_t val = digitalRead(BUSY_PIN);
  return 1 - val;
}
