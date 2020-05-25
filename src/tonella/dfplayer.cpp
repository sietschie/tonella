#include "dfplayer.h"

#include "SoftwareSerial.h"

SoftwareSerial serialDfPlayer(2, 3); // RX, TX

bool DFPlayer::init() {
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

  return true;
}

void DFPlayer::pause() { myDFPlayer.pause(); }

void DFPlayer::resume() { myDFPlayer.start(); }

void DFPlayer::play(uint16_t folder, uint16_t index) {
  myDFPlayer.playFolder(folder, index);
}

void DFPlayer::set_volume(uint8_t volume) { myDFPlayer.volume(volume); }
