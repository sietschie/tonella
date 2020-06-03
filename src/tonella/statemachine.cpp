#include "statemachine.h"
#include "Arduino.h"

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
    } else if (play_state == 0) {
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
    break;
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
  Serial.print("execute command periodically, time = ");
  Serial.println(current_time);

  int32_t time_diff = current_time - command_last_active_time;
  if (time_diff > 500) {
    command_last_active_time = current_time;
    execute_command(command);
  }
}

void StateMachine::execute_command(uint16_t index) {
  if (index == COMMAND_VOLUME_UP_ID) {
    uint8_t current_volume = player->get_volume();
    if (current_volume >= 30)
    {
      player->stop();
      return;
    }

    uint8_t new_volume = current_volume + 1;
    player->set_volume(new_volume);
    led->start(ILed::Mode::Volume, 1210, new_volume - 6);

    if (!command_periodically_active) {
      command_start_time = system->get_timestamp() + 500;
      command_last_active_time = command_start_time;
      command_periodically_active = true;
      command = index;
      player->play_beeps();
    }
    logger->print(ILogger::Debug, "Volume ");
    logger->print(ILogger::Debug, (int)new_volume);
    logger->println(ILogger::Debug);
  } else if (index == COMMAND_VOLUME_DOWN_ID) {
    uint8_t current_volume = player->get_volume();
    if (current_volume <= 7)
    {
      player->stop();
      return;
    }

    uint8_t new_volume = current_volume - 1;
    player->set_volume(new_volume);
    led->start(ILed::Mode::Volume, 1210, new_volume - 6);

    if (!command_periodically_active) {
      command_start_time = system->get_timestamp() + 500;
      command_last_active_time = command_start_time;
      command_periodically_active = true;
      command = index;
      player->play_beeps();
    }
    logger->print(ILogger::Debug, "Volume ");
    logger->print(ILogger::Debug, (int)new_volume);
    logger->println(ILogger::Debug);
  } else if (index == COMMAND_MODE_CHANGE) {
    led->set(0, 1, 0);
    led->start(ILed::Mode::Wipe, 1010);
    player->change_to_next_mode();
  }
}
