#include "Arduino.h"


#define NFC_TAG_UNCHANGED 0
#define NFC_TAG_FOUND     1
#define NFC_TAG_GONE      2

bool readCard(byte* index);
byte checkCardStatus(byte* index);
void nfc_init();
