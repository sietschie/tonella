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

void nfc_init() {
  // Init NFC
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  // delay(4);       // Optional delay. Some board do need more time after init
  // to be ready, see Readme mfrc522.PCD_DumpVersionToSerial();  // Show details
  // of PCD - MFRC522 Card Reader details Serial.println(F("Scan PICC to see
  // UID, SAK, type, and data blocks...")); Prepare the key (used both as key A
  // and as key B) using FFFFFFFFFFFFh which is the default at chip delivery
  // from the factory
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // Serial.println(F("RC522 ready."));
}

bool readCard(byte *index) {
  // mfrc522.PICC_IsNewCardPresent();
  // mfrc522.PICC_ReadCardSerial();

  Serial.print("PICC Type = ");
  Serial.println(mfrc522.PICC_GetType(mfrc522.uid.sak));

  // In this sample we use the second sector,
  // that is: sector #1, covering block #4 up to and including block #7
  byte sector = 1;
  byte trailerBlock = 7;
  byte blockAddr = 4;
  MFRC522::StatusCode status;
  int32_t tagNr;

  if (mfrc522.PICC_GetType(mfrc522.uid.sak) ==
      4) // don't authenticate for mifare ultralight tags
  {
    // Authenticate using key A
    Serial.println();
    // Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      // return false;
    }
  }

  // Read data from the block
  Serial.print(F("Reading data from block "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));
  status = (MFRC522::StatusCode)mfrc522.MIFARE_GetValue(blockAddr, &tagNr);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  // Serial.print(F("tagNr ")); Serial.print(tagNr);

  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  if (tagNr > 255 || tagNr < 0)
    return false;

  index[0] = tagNr;
  return true;
}

byte checkCardStatus(byte *index) {
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
      return NFC_TAG_UNCHANGED;
    }
    _rfid_error_counter = 0;
    _tag_found = true;
  }

  rfid_tag_present = _tag_found;

  byte status = NFC_TAG_UNCHANGED;
  // rising edge
  if (rfid_tag_present && !rfid_tag_present_prev) {
    // Serial.println("Tag found");
    status = NFC_TAG_FOUND;
    readCard(index);
    mfrc522.PCD_Reset();
    delay(100);
    mfrc522.PCD_Init(); // Init MFRC522
  }

  // falling edge
  if (!rfid_tag_present && rfid_tag_present_prev) {
    // Serial.println("Tag gone");
    status = NFC_TAG_GONE;
  }

  return status;
}
