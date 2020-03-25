#include "led.h"
#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN 6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 24 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

bool running = false;
byte current_mode = LED_MODE_NONE;
unsigned long time_start = 0;
unsigned long time_duration = 0;
int payload = 0;

void led_init() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void led_start(byte mode, unsigned long duration, int payload_) {
  running = true;
  current_mode = mode;
  time_start = millis();
  time_duration = duration;
  payload = payload_;
}

bool update_breath() {
  unsigned long now = millis();
  if (now > time_start + time_duration) {
    pixels.clear();
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
  for (int index_pixel = 0; index_pixel < NUMPIXELS; index_pixel++) {
    pixels.setPixelColor(index_pixel,
                         pixels.Color(brightness, brightness, brightness));
  }
  pixels.show();

  return true;
}

bool update_volume() {
  unsigned long now = millis();
  if (now > time_start + time_duration) {
    pixels.clear();
    pixels.show();
    return false;
  }

  unsigned long time_elapsed = now - time_start;
  unsigned long ticks = time_elapsed / 100; // one revolution per second

  pixels.clear();
  for (int index_pixel = 0; index_pixel < NUMPIXELS; index_pixel++) {
    if (index_pixel < payload) {
      pixels.setPixelColor(index_pixel, pixels.Color(20, 20, 20));
    } else {
      pixels.setPixelColor(index_pixel, pixels.Color(0, 0, 0));
    }
  }
  pixels.show();

  return true;
}
bool update_blink() {
  unsigned long now = millis();
  if (now > time_start + time_duration) {
    pixels.clear();
    pixels.show();
    return false;
  }

  unsigned long time_elapsed = now - time_start;
  unsigned long ticks = time_elapsed / 100; // one revolution per second

  pixels.clear();
  for (int index_pixel = 0; index_pixel < NUMPIXELS; index_pixel++) {
    if (ticks % 2 == 0) {
      pixels.setPixelColor(index_pixel, pixels.Color(20, 20, 20));
    } else {
      pixels.setPixelColor(index_pixel, pixels.Color(0, 0, 0));
    }
  }
  pixels.show();

  return true;
}

bool update_wipe() {
  unsigned long now = millis();
  if (now > time_start + time_duration) {
    pixels.clear();
    pixels.show();
    return false;
  }

  unsigned long time_elapsed = now - time_start;
  unsigned long ticks =
      (time_elapsed * NUMPIXELS) / 500; // one revolution per second

  byte index_pixel = ticks % NUMPIXELS;

  if (ticks < 25)
    pixels.clear();
  pixels.setPixelColor((index_pixel + NUMPIXELS - 1) % NUMPIXELS,
                       pixels.Color(10, 10, 10));
  pixels.setPixelColor(index_pixel, pixels.Color(20, 20, 20));
  pixels.setPixelColor((index_pixel + 1) % NUMPIXELS, pixels.Color(10, 10, 10));
  pixels.show();

  return true;
}

void led_set(byte r, byte g, byte b)
{
  for (int index_pixel = 0; index_pixel < NUMPIXELS; index_pixel++) {
    pixels.setPixelColor(index_pixel, pixels.Color(r, g, b));
  }
  pixels.show();
}

void led_loop() {
  if (running) {
    bool res;
    if (current_mode == LED_MODE_WIPE) {
      res = update_wipe();
    } else if (current_mode == LED_MODE_BLINK) {
      res = update_blink();
    } else if (current_mode == LED_MODE_BREATH) {
      res = update_breath();
    } else if (current_mode == LED_MODE_VOLUME) {
      res = update_volume();
    }
    if (!res) {
      running = false;
    }
  }
}
