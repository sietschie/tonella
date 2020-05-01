#pragma once

#include "Arduino.h"
#include "iled.h"
#include <Adafruit_NeoPixel.h>

/**
 * led class controls LED state and animations
 */
class Led : public ILed {
private:
  uint8_t pin;
  uint8_t num_pixels;
  uint8_t r, g, b;

  Adafruit_NeoPixel pixels;

  bool running = false;
  byte current_mode = Led::Mode::None;
  unsigned long time_start = 0;
  unsigned long time_duration = 0;
  int payload = 0;

  bool update_breath();
  bool update_volume();
  bool update_blink();
  bool update_wipe();

  void reset_pixels();

public:
  Led();
  void init() override;
  void loop() override;
  void start(Mode mode, uint32_t duration, uint16_t payload = 0) override;
  void set(uint8_t r, uint8_t g, uint8_t b) override;
};
