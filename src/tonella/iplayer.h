#pragma once

/**
 * interface to player class
 *
 * controls mp3 player module
 */
class IPlayer {
public:
  /**
   * different player modes
   *
   * Sound   playes a sound and says the name of the figurine
   * Song    plays a song associated with the figurine
   */
  enum Mode { Sound, Song };

  /**
   * initialize reader
   *
   * @returns true if initialized succesfully
   */
  virtual bool init() = 0;

  /** pause current song */
  virtual void pause() = 0;

  /**
   * pause current song
   *
   * @param index   index of song to play
   */
  virtual void play(uint16_t index) = 0;

  /**
   * set volume
   *
   * @param volume   volume between 0 and 30
   */
  virtual void set_volume(uint8_t volume) = 0;

  /**
   * get current volume
   *
   * @returns current volume
   */
  virtual uint8_t get_volume() = 0;

  /**
   * sets current player mode
   *
   * @param mode  new mode
   */
  virtual void set_mode(Mode mode) = 0;

  /**
   * return current player mode
   *
   * @returns current player mode
   */
  virtual Mode get_mode() = 0;
};
