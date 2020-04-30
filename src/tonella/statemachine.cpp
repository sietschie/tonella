#include "statemachine.h"

void StateMachine::init(ILed *_led, INfc *_nfc, IPlayer *_player) {
  led = _led;
  nfc = _nfc;
  player = _player;
}

void StateMachine::run() {
  uint16_t index = 0;
  INfc::Type type;

  INfc::TagState nfc_status = nfc->checkCardStatus(type, index);

  switch (state) {
  case IDLE:
    if (nfc_status == INfc::TAG_FOUND) {
      if (type == INfc::Type::FIGURINE) {
        play_song(index);
        state = PLAYING;
      } else if (type == INfc::Type::COMMAND) {
        execute_command(index);
        state = COMMAND;
      }
    }
    break;
  case PLAYING:
    if (nfc_status == INfc::TAG_GONE) {
      stop_song();
      state = IDLE;
    };
    break;
  case COMMAND:
    if (nfc_status == INfc::TAG_GONE) {
      state = IDLE;
    };
    break;
  };
}

void StateMachine::play_song(int index) {
  player->play(index);
  led->start(ILed::Mode::Wipe, 1010);
}

void StateMachine::stop_song() {
  player->pause();
  led->start(ILed::Mode::Blink, 800);
}

void StateMachine::execute_command(int index) {
  if (index == COMMAND_VOLUME_UP_ID) {
    player->set_volume(player->get_volume() + 3);
    led->start(ILed::Mode::Volume, 1010, player->get_volume() - 6);
    // Serial.print("Volume ");
    // Serial.println(player.get_volume());
  } else if (index == COMMAND_VOLUME_DOWN_ID) {
    player->set_volume(player->get_volume() - 3);
    led->start(ILed::Mode::Volume, 1010, player->get_volume() - 6);
    // Serial.print("Volume ");
    // Serial.println(player.get_volume());
  } else if (index == COMMAND_MODE_CHANGE) {
    player->set_mode(1 - player->get_mode());
  }
}
