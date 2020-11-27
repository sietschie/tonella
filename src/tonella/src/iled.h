#pragma once

#include <stdint.h>

/**
 * Interface to led class
 *
 * Controls LED state and animations
 */
class ILed {
public:
  /**
   * supported animation modes
   *
   * None - no animation
   * Wipe - rotating animation
   * Blink - all leds turn on and off
   * Breath - all leds turn smoothly on and off
   * Volume - n leds are turned on to visualize current volume
   */
  enum Mode { None, Wipe, Blink, Breath, Volume };

  /** initialisation */
  virtual void init() = 0;

  /** needs to be called periodically to run animations*/
  virtual void loop() = 0;

  /** start an animation
   *
   * @param mode       selected animation
   * @param duration   length of the animation in milliseconds
   * @param payload    animation specific parameter
   */
  virtual void start(Mode mode, uint32_t duration, uint16_t payload = 0) = 0;

  /** set all leds to specified color
   *
   * @param r   amount of red
   * @param g   amount of green
   * @param b   amount of blye
   */
  virtual void set(uint8_t r, uint8_t g, uint8_t b) = 0;
};
