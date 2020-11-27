#pragma once

#include "Arduino.h"
#include "infc.h"

/**
 * nfc class controls nfc module
 */
class Nfc : public INfc {
public:
  bool readCard(Type &type, uint16_t &index) override;
  TagState checkCardStatus(Type &type, uint16_t &index) override;
  bool init() override;
};
