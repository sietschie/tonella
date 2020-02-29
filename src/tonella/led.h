#include "Arduino.h"

#define LED_MODE_NONE     0
#define LED_MODE_WIPE     1
#define LED_MODE_BLINK    2
#define LED_MODE_BREATH   3
#define LED_MODE_VOLUME   4

void led_init();
void led_loop();
void led_start(byte mode, unsigned long duration, int payload=0);
