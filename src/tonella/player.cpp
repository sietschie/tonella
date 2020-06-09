#include "player.h"

bool Player::init(IDFPlayer *_dfplayer, IMemory *_memory) {
  dfplayer = _dfplayer;
  memory = _memory;

  volume = memory->get_volume();
  dfplayer->set_volume(volume);

  return true;
}

void Player::play(uint16_t index) {
  if (mode == current_mode && index == current_track) {
    dfplayer->resume();
  } else if (mode == Metal) {
    dfplayer->play(5, index);
    current_mode = mode;
    current_track = index;
  } else if (mode == Story) {
    dfplayer->play(4, index);
    current_mode = mode;
    current_track = index;
  } else if (mode == Sound) {
    dfplayer->play(3, index);
    current_mode = mode;
    current_track = index;
  } else {
    dfplayer->play(2, index);
    current_mode = mode;
    current_track = index;
  }
}

void Player::pause() { dfplayer->pause(); }

void Player::play_beeps() { dfplayer->play(1, 4); }

void Player::stop() {
  dfplayer->pause();
  current_track = 0;
}

void Player::set_volume(uint8_t _volume) {
  volume = _volume;
  memory->save_volume(volume);
  dfplayer->set_volume(volume);
}

uint8_t Player::get_volume() { return volume; }

Player::Mode Player::get_mode() { return mode; }

void Player::change_to_next_mode() {
  if (mode == Metal) {
    mode = Song;
    dfplayer->play(1, 1);
  } else if (mode == Song) {
    mode = Sound;
    dfplayer->play(1, 2);
  } else if (mode == Sound) {
    mode = Story;
    dfplayer->play(1, 5);
  } else if (mode == Story) {
    mode = Metal;
    dfplayer->play(1, 6);
  }
}
