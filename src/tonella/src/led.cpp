#include "led.h"

Led::Led()
    : pin(6), num_pixels(24), pixels(num_pixels, pin, NEO_GRB + NEO_KHZ800) {}

void Led::init() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void Led::start(Mode mode, uint32_t duration, uint16_t payload) {
  running = true;
  current_mode = mode;
  time_start = millis();
  time_duration = duration;
  this->payload = payload;
}

bool Led::update_breath() {
  unsigned long now = millis();
  if (now > time_start + time_duration) {
    reset_pixels();
    pixels.show();
    return false;
  }

  unsigned long time_elapsed = now - time_start;
  unsigned long ticks =
      80 * time_elapsed / time_duration; // one revolution per second

  int brightness = ticks % 40;
  if (brightness > 20)
    brightness = 40 - brightness;

  pixels.clear();
  for (int index_pixel = 0; index_pixel < num_pixels; index_pixel++) {
    pixels.setPixelColor(index_pixel,
                         pixels.Color(brightness, brightness, brightness));
  }
  pixels.show();

  return true;
}

bool Led::update_volume() {
  unsigned long now = millis();
  if (now > time_start + time_duration) {
    reset_pixels();
    pixels.show();
    return false;
  }

  unsigned long time_elapsed = now - time_start;
  unsigned long ticks = time_elapsed / 100; // one revolution per second

  pixels.clear();
  for (int index_pixel = 0; index_pixel < num_pixels; index_pixel++) {
    if (index_pixel < payload) {
      pixels.setPixelColor(index_pixel, pixels.Color(20, 20, 20));
    } else {
      pixels.setPixelColor(index_pixel, pixels.Color(0, 0, 0));
    }
  }
  pixels.show();

  return true;
}
bool Led::update_blink() {
  unsigned long now = millis();
  if (now > time_start + time_duration) {
    reset_pixels();
    pixels.show();
    return false;
  }

  unsigned long time_elapsed = now - time_start;
  unsigned long ticks = time_elapsed / 100; // one revolution per second

  pixels.clear();
  for (int index_pixel = 0; index_pixel < num_pixels; index_pixel++) {
    if (ticks % 2 == 0) {
      pixels.setPixelColor(index_pixel, pixels.Color(20, 20, 20));
    } else {
      pixels.setPixelColor(index_pixel, pixels.Color(0, 0, 0));
    }
  }
  pixels.show();

  return true;
}

bool Led::update_wipe() {
  unsigned long now = millis();
  if (now > time_start + time_duration) {
    reset_pixels();
    pixels.show();
    return false;
  }

  unsigned long time_elapsed = now - time_start;
  unsigned long ticks =
      (time_elapsed * num_pixels) / 500; // one revolution per second

  byte index_pixel = ticks % num_pixels;

  if (ticks < 25)
    pixels.clear();
  pixels.setPixelColor((index_pixel + num_pixels - 1) % num_pixels,
                       pixels.Color(10, 10, 10));
  pixels.setPixelColor(index_pixel, pixels.Color(20, 20, 20));
  pixels.setPixelColor((index_pixel + 1) % num_pixels,
                       pixels.Color(10, 10, 10));
  pixels.show();

  return true;
}

void Led::set(byte r, byte g, byte b) {
  this->r = r;
  this->g = g;
  this->b = b;
  for (int index_pixel = 0; index_pixel < num_pixels; index_pixel++) {
    pixels.setPixelColor(index_pixel, pixels.Color(r, g, b));
  }
  pixels.show();
}

void Led::reset_pixels() { set(r, g, b); }

void Led::loop() {
  if (running) {
    bool res;
    if (current_mode == Mode::Wipe) {
      res = update_wipe();
    } else if (current_mode == Mode::Blink) {
      res = update_blink();
    } else if (current_mode == Mode::Breath) {
      res = update_breath();
    } else if (current_mode == Mode::Volume) {
      res = update_volume();
    }
    if (!res) {
      running = false;
    }
  }
}
