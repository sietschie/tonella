#include "Arduino.h"
#include "led.h"
#include "nfc.h"
#include "player.h"
#include "statemachine.h"
#include "system.h"

Player player;
Nfc nfc;
Led led;
System sys;
StateMachine state_machine;

void tick() { led.loop(); }

// wait - but tick while waiting
void idle(uint16_t sleep) {
  long start = millis();
  while ((start + sleep) > millis()) {
    tick();
    delay(5);
  }
}

void setup() {
  Serial.begin(115200);

  // Init LED
  Serial.print(F("Init LED: "));
  led.init();
  led.set(0, 0, 1);
  Serial.println(F("done"));

  Serial.print(F("Init NFC: "));
  if (!nfc.init()) {
    Serial.println(F("Communication failure"));
    led.set(0, 10, 10); // show cyan on error
    while (true)
      ;
  }
  Serial.println(F("done"));

  Serial.print(F("Init Player: "));
  if (!player.init()) {
    Serial.println(F("failure"));
    led.set(10, 10, 0); // show yellow on error
    while (true)
      ;
  }
  Serial.println(F("done"));

  state_machine.init(&led, &nfc, &player, &sys);

  led.set(1, 0, 0);
  led.start(Led::Mode::Breath, 2000);
  idle(2000);
}

void loop(void) {
  state_machine.run();
  idle(100);
}
