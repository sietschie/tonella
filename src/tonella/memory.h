#include <Arduino.h>

/**
 * Interface to memory class
 *
 * Using this class persistent information like volume
 * can be safed and retreived.
 */
class IMemory {
public:
  /**
   * return saved volume
   *
   * @returns saved volume
   */
  virtual byte get_volume() = 0;

  /**
   * save volume to memory
   *
   * @param volume   the volume to be saved
   */
  virtual void save_volume(uint8_t volume) = 0;
};

/**
 * Memory class to save and retreive data from eeprom
 */
class Memory : public IMemory {
public:
  byte get_volume() override;
  void save_volume(uint8_t volume) override;
};
