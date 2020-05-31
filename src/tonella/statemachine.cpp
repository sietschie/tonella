#include "statemachine.h"

void StateMachine::init(ILed *_led, INfc *_nfc, IPlayer *_player,
                        ISystem *_system, ILogger *_logger) {
  led = _led;
  nfc = _nfc;
  player = _player;
  system = _system;
  logger = _logger;
}

void StateMachine::run() {
  uint16_t index = 0;
  INfc::Type type;

  INfc::TagState nfc_status = nfc->checkCardStatus(type, index);
  uint8_t play_state = system->get_play_state();

  switch (state) {
  case IDLE:
    if (nfc_status == INfc::TAG_FOUND) {
      if (type == INfc::Type::FIGURINE) {
        play_song(index);
        state = PLAYING;
        logger->println(ILogger::Info, "changed state from IDLE to PLAYING");
        led->set(0, 1, 0);
      } else if (type == INfc::Type::COMMAND) {
        execute_command(index);
        state = COMMAND;
        logger->println(ILogger::Info, "changed state from IDLE to COMMAND");
      }
    }
    break;
  case PLAYING:
    if (nfc_status == INfc::TAG_GONE) {
      stop_song();
      state = IDLE;
      logger->println(ILogger::Info, "changed state from PLAYING to IDLE");
      led->set(1, 0, 0);
    };
    if (play_state == 0) {
      state = DONE;
      logger->println(ILogger::Info, "changed state from PLAYING to DONE");
      led->set(1, 1, 0);
    }
    break;
  case DONE:
    if (nfc_status == INfc::TAG_GONE) {
      stop_song();
      state = IDLE;
      logger->println(ILogger::Info, "changed state from DONE to IDLE");
      led->set(1, 0, 0);
    };
  case COMMAND:
    if (nfc_status == INfc::TAG_GONE) {
      state = IDLE;
      logger->println(ILogger::Info, "changed state from COMMAND to IDLE");
      player->stop();
      command_periodically_active = false;
      led->set(1, 0, 0);
    } else {
      if (command_periodically_active) {
        execute_command_periodically();
      }
    }
    break;
  };
}

void StateMachine::play_song(uint16_t index) {
  player->play(index);
  led->start(ILed::Mode::Wipe, 1010);
}

void StateMachine::stop_song() {
  player->pause();
  led->start(ILed::Mode::Blink, 800);
}

void StateMachine::execute_command_periodically() {
  uint32_t current_time = system->get_timestamp();

  int32_t time_diff = current_time - command_last_active_time;
  if (time_diff > 100) {
    command_last_active_time = current_time;
    execute_command(command);
  }
}

void StateMachine::execute_command(uint16_t index) {
  if (index == COMMAND_VOLUME_UP_ID) {
    player->set_volume(player->get_volume() + 1);
    led->start(ILed::Mode::Volume, 1010, player->get_volume() - 6);

    command_start_time = system->get_timestamp() + 500;
    command_last_active_time = system->get_timestamp() + 500;
    command_periodically_active = true;
    command = index;
    // Serial.print("Volume ");
    // Serial.println(player.get_volume());
  } else if (index == COMMAND_VOLUME_DOWN_ID) {
    player->set_volume(player->get_volume() - 1);
    led->start(ILed::Mode::Volume, 1010, player->get_volume() - 6);

    command_start_time = system->get_timestamp() + 500;
    command_last_active_time = system->get_timestamp() + 500;
    command_periodically_active = true;
    command = index;
    // Serial.print("Volume ");
    // Serial.println(player.get_volume());
  } else if (index == COMMAND_MODE_CHANGE) {
    led->set(0, 1, 0);
    led->start(ILed::Mode::Wipe, 1010);
    player->change_to_next_mode();
  }
}
