#include "nfc.h"

#include <MFRC522.h>

// NFC Board
#define RST_PIN 9                 // Configurable, see typical pin layout above
#define SS_PIN 10                 // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
MFRC522::MIFARE_Key key;
bool rfid_tag_present_prev = false;
bool rfid_tag_present = false;
int _rfid_error_counter = 0;
bool _tag_found = false;
#define ULTRALIGHT_DATA_START_PAGE 4

bool Nfc::init() {
  // Init NFC
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522

  // check communication
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    return false;
  }

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  return true;
}

bool Nfc::readCard(Type &type, uint16_t &index) {
  byte ultraLightReadSize = 18;
  byte data[ultraLightReadSize];
  byte size = ultraLightReadSize;
  int status = mfrc522.MIFARE_Read(ULTRALIGHT_DATA_START_PAGE, data, &size);

  int messageLength = 0;
  int messageStart = 0;
  if (status == MFRC522::StatusCode::STATUS_OK) {
    if (data[0] == 0x03) {
      messageLength = data[1];
    } else if (data[5] == 0x03) {
      messageLength = data[6];
      messageStart = 5;
    }
  }

  byte recordLength = data[messageStart + 4];

  if (data[messageStart + 2] != 0x91 || data[messageStart + 3] != 0x01) {
    Serial.print("type does not match: ");
    Serial.print(data[messageStart + 2], HEX);
    Serial.print(" ");
    Serial.print(data[messageStart + 3], HEX);
    Serial.println(" (expected 91 and 1)");
    return false;
  }

  if (messageStart + recordLength + 6 >= ultraLightReadSize) {
    Serial.println("record ends beyond buffer size");
    return false;
  }

  // get payload
  data[messageStart + recordLength + 6] = '\0';
  char *payload = (char *)data + messageStart + 7;

  Serial.print("read card payload = ");
  Serial.println(payload);

  // parse payload
  switch (payload[2]) {
  case 'F':
    type = INfc::Type::FIGURINE;
    break;
  case 'C':
    type = INfc::Type::COMMAND;
    break;
  }

  String id((char *)payload + 3);
  index = id.toInt();

  return true;
}

Nfc::TagState Nfc::checkCardStatus(Type &type, uint16_t &index) {
  rfid_tag_present_prev = rfid_tag_present;

  _rfid_error_counter += 1;
  if (_rfid_error_counter > 2) {
    _tag_found = false;
  }

  // Detect Tag without looking for collisions
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);

  // Reset baud rates
  mfrc522.PCD_WriteRegister(mfrc522.TxModeReg, 0x00);
  mfrc522.PCD_WriteRegister(mfrc522.RxModeReg, 0x00);
  // Reset ModWidthReg
  mfrc522.PCD_WriteRegister(mfrc522.ModWidthReg, 0x26);

  MFRC522::StatusCode result = mfrc522.PICC_RequestA(bufferATQA, &bufferSize);

  if (result == mfrc522.STATUS_OK) {
    if (!mfrc522.PICC_ReadCardSerial()) { // Since a PICC placed get Serial and
                                          // continue
      return TAG_UNCHANGED;
    }
    _rfid_error_counter = 0;
    _tag_found = true;
  }

  rfid_tag_present = _tag_found;

  TagState status = TAG_UNCHANGED;
  // rising edge
  if (rfid_tag_present && !rfid_tag_present_prev) {
    // Serial.println("Tag found");
    status = TAG_FOUND;
    readCard(type, index);
    mfrc522.PCD_Reset();
    delay(100);
    mfrc522.PCD_Init(); // Init MFRC522
  }

  // falling edge
  if (!rfid_tag_present && rfid_tag_present_prev) {
    // Serial.println("Tag gone");
    status = TAG_GONE;
  }

  return status;
}
