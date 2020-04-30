#include "player.h"

#include "SoftwareSerial.h"
#include "memory.h"

Memory memory;

SoftwareSerial serialDfPlayer(2, 3); // RX, TX

bool Player::init() {
  // DFPlayer Mini

  Serial.print(F("Init DFPlayer: "));
  serialDfPlayer.begin(9600);

  if (!myDFPlayer.begin(
          serialDfPlayer)) { // Use softwareSerial to communicate with mp3.
    Serial.println(F(""));
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    return false;
  }
  Serial.println(F("done"));

  Serial.print(F("Read saved Volume: "));
  volume = memory.get_volume();
  myDFPlayer.volume(volume); // Set volume value. From 0 to 30
  Serial.print("set volume to ");
  Serial.println(volume);

  return true;
}

void Player::play(uint16_t index) {
  if (mode == current_mode && index == current_track) {
    myDFPlayer.start();
  } else if (mode == 0) {
    myDFPlayer.playFolder(3, index);
    current_mode = mode;
    current_track = index;
  } else {
    myDFPlayer.playFolder(2, index);
    current_mode = mode;
    current_track = index;
  }
}

void Player::pause() { myDFPlayer.pause(); }

void Player::set_volume(uint8_t _volume) {
  volume = _volume;
  memory.save_volume(volume);
  myDFPlayer.playFolder(1, 3);
}

uint8_t Player::get_volume() { return volume; }

Player::Mode Player::get_mode() { return mode; }

void Player::set_mode(Mode mode) { this->mode = mode; }
