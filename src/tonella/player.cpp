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
  } else if (mode == 0) {
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

void Player::set_volume(uint8_t _volume) {
  volume = _volume;
  memory->save_volume(volume);
  dfplayer->set_volume(volume);
  dfplayer->play(1, 3);
}

uint8_t Player::get_volume() { return volume; }

Player::Mode Player::get_mode() { return mode; }

void Player::change_to_next_mode() {
  if (mode == Sound)
    mode = Song;
  else if (mode == Song)
    mode = Sound;
}
