#include "nfc.h"

#include <MFRC522.h>
#include <MifareUltralight.h>

// NFC Board
#define RST_PIN 9                 // Configurable, see typical pin layout above
#define SS_PIN 10                 // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
MFRC522::MIFARE_Key key;
bool rfid_tag_present_prev = false;
bool rfid_tag_present = false;
int _rfid_error_counter = 0;
bool _tag_found = false;

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

  ndef_mfrc522::MifareUltralight reader =
      ndef_mfrc522::MifareUltralight(mfrc522);
  ndef_mfrc522::NfcTag tag = reader.read();
  ndef_mfrc522::NdefMessage msg = tag.getNdefMessage();

  if (msg.getRecordCount() < 3) {
    Serial.println("NFC: Not enough records found on rfid chip");
    type = 'e';
    index = 1;
    return false;
  }

  ndef_mfrc522::NdefRecord rcd = msg.getRecord(0);
  char bufferType[2];
  rcd.getType(bufferType);
  if (bufferType[0] != 0x54) {
    Serial.println("NFC: Unexpected record type");
    type = 'e';
    index = 2;
    return false;
  }

  // read and parse record 0
  byte payload[rcd.getPayloadLength() + 1];
  rcd.getPayload(payload);

  switch (payload[3]) {
  case 'F':
    type = INfc::Type::FIGURINE;
    break;
  case 'C':
    type = INfc::Type::COMMAND;
    break;
  }

  payload[rcd.getPayloadLength()] = '\0'; // add zero termination
  String id((char *)payload + 4);
  index = id.toInt();

  {
    ndef_mfrc522::NdefRecord rcd = msg.getRecord(1);
    byte payload[rcd.getPayloadLength() + 1];
    rcd.getPayload(payload);
    payload[rcd.getPayloadLength()] = '\0'; // add zero termination
    String figurineName((char *)payload + 3);
    Serial.println(figurineName);
  }

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
