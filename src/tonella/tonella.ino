#include "Arduino.h"
#include "dfplayer.h"
#include "led.h"
#include "logger.h"
#include "memory.h"
#include "nfc.h"
#include "nulllogger.h"
#include "player.h"
#include "statemachine.h"
#include "system.h"
#include "version.h"

DFPlayer dfplayer;
Player player;
Memory memory;
Nfc nfc;
Led led;
System sys;
StateMachine state_machine;
Logger logger(ILogger::Debug);

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
  sys.init();
  logger.init();
  logger.print(ILogger::Info, "Version: ");
  logger.println(ILogger::Info, build_version);
  logger.print(ILogger::Info, "Build date: ");
  logger.println(ILogger::Info, build_date);

  // Init LED
  logger.print(ILogger::Info, "Init LED: ");
  led.init();
  led.set(1, 1, 1);
  logger.println(ILogger::Info, "done");

  logger.print(ILogger::Info, "Init NFC: ");
  if (!nfc.init()) {
    logger.println(ILogger::Info, "Communication failure");
    led.set(0, 10, 10); // show cyan on error
    while (true)
      ;
  }
  logger.println(ILogger::Info, "done");

  if (!dfplayer.init() || !player.init(&dfplayer, &memory)) {
    Serial.println(F("failure"));
    led.set(10, 10, 0); // show yellow on error
    while (true)
      ;
  }

  state_machine.init(&led, &nfc, &player, &sys, &logger);

  led.set(1, 0, 0);
  led.start(Led::Mode::Breath, 2000);
  idle(2000);
}

void loop(void) {
  state_machine.run();
  idle(100);
}
