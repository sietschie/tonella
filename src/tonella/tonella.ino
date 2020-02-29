#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include "nfc.h"
#include "led.h"
#include "memory.h"

#define STATE_STOPPED    0
#define STATE_PLAYING    1


int state = STATE_STOPPED;
byte currentTag = 0;
byte volume = 0;

#define COMMAND_VOLUME_DOWN_ID      7
#define COMMAND_VOLUME_UP_ID        8
#define COMMAND_MODE_CHANGE         6

byte currentMode = 0;

// DFPlayer Mini
SoftwareSerial serialDfPlayer(3, 2); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

void tick() {
  led_loop();
}

// wait - but tick while waiting
void idle(uint16_t sleep) {
  long start = millis();
  while ((start + sleep) > millis()) {
    tick();
    delay(5);
  }
}


void setup()
{
  Serial.begin(115200);

  // Init LED
  led_init();

  nfc_init();

  Serial.println(F("Start DFPlayer"));
  serialDfPlayer.begin(9600);

  if (!myDFPlayer.begin(serialDfPlayer)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  volume = memory_get_volume();
  Serial.print("volume = ");
  Serial.println(volume);
  myDFPlayer.volume(volume);  //Set volume value. From 0 to 30
  //myDFPlayer.play(1);  //Play the first mp3

  led_start(LED_MODE_BREATH, 2000);
  idle(2000);
}


void loop(void) {
  idle(100);

  byte index = 0;
  byte nfc_status = checkCardStatus(&index);
  if( nfc_status == NFC_TAG_FOUND )
  {
    Serial.println("Tag found");
    Serial.println(index);
    if( index == COMMAND_MODE_CHANGE )
    {
      currentMode = 1 - currentMode;
        Serial.print("currentMode = ");
        Serial.println(currentMode);
    }

    if( index == COMMAND_VOLUME_UP_ID || index == COMMAND_VOLUME_DOWN_ID )
    {
        if( index == COMMAND_VOLUME_DOWN_ID && volume > 6) volume -= 3;
        if( index == COMMAND_VOLUME_UP_ID && volume < 30) volume += 3;
        myDFPlayer.volume(volume);
        led_start(LED_MODE_VOLUME, 1010, volume - 6);
        Serial.print("Volume ");
        Serial.println(volume);
        memory_save_volume(volume);
        myDFPlayer.playFolder(1,3);
        return;
    }

    if( currentMode == 0 )
    {
      if( currentTag == index )
      {
        myDFPlayer.start();
      } else {
        myDFPlayer.playFolder(3,index);
      }
      state = STATE_PLAYING;
      currentTag = index;
    } else {
        myDFPlayer.playFolder(2,index);
    }
    
    led_start(LED_MODE_WIPE, 1010);
    
  } else if( nfc_status == NFC_TAG_GONE )
  {
    Serial.println("Tag gone");
    if( state == STATE_PLAYING )
    {
      myDFPlayer.pause();
      led_start(LED_MODE_BLINK, 800);
      state = STATE_STOPPED;
    }
  }
}
