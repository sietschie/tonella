#pragma once

/**
 * interface for nfc class
 *
 * controls nfc module
 */
class INfc {
public:
  /**
   * nfc reader states
   *
   * TAG_UNCHANGED  nothing has changed since previous call
   * TAG_FOUND      since the last call a new tag has appeared
   * TAG_GONE       since the last call a tag was removed from reader
   */
  enum TagState { TAG_UNCHANGED, TAG_FOUND, TAG_GONE };

  /**
   * COMMAND    command card that changes the state of the box
   * FIGURINE   figurine card that starts a song
   */
  enum Type { COMMAND, FIGURINE };

  /**
   * get id from rfid card
   *
   * @param   type   returns type id of card
   * @param   index  returns index of card
   *
   * @returns true if read succesfully
   */
  virtual bool readCard(Type &type, uint16_t &index) = 0;

  /**
   * check reader status
   *
   * @param   type   returns type id of card
   * @param   index  returns index of card
   *
   * @returns reader state
   */
  virtual TagState checkCardStatus(Type &type, uint16_t &index) = 0;

  /**
   * initialize reader
   *
   * @returns true if initialized succesfully
   */
  virtual bool init() = 0;
};
