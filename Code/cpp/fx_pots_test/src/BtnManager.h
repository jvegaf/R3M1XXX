#pragma once
#include "base.h"
#include <Wire.h>
#include <ht16k33.h>

// AIDEV-NOTE: BtnManager handles button reading from HT16K33 key scanning
// The HT16K33 can scan up to 13x3 keys (ROW0-ROW12 x COM0-COM2)
// We use it for multiple buttons connected to the key matrix
class BtnManager {
private:
  HT16K33 &ht;

public:
  // AIDEV-NOTE: Constructor initializes button manager with HT16K33 reference,
  // starting MIDI note, button positions array, number of buttons, I2C address,
  // and debug name
  BtnManager(HT16K33 &ic) : ht(ic) {}

  void update(void (*func)(uint8_t, BtnState)) {
    static uint8_t lastKey = 0;
    int8_t key;

    /****************************************************************
     * read the keys and return the key that changed state
     * if more than one is pressed (compared to last scan)
     * only one is returned, the first one found
     * 0 means no key pressed.
     * "1" means the key #1 is pressed
     * "-1" means the key #1 is released
     * "clear"=true means it will only look keys currently pressed down.
     *     this is so you can detect what key is still pressed down after
     *     several keys are pressed down and then all but one is released
     *     (without keeping track of up/down separately)
     *
     *Observations:
     * As long as the key is pressed the keyram bit is set
     * the flag is set when key is pressed down but then cleared at first
     * read of key ram.
     * When released the key corresponding bit is cleared but the flag is NOT
     * set This means that the only way a key release can be detected is by only
     * polling readKey and ignoring flag
     *
     */

    key = ht.readKey();
    if (key > 0) { // key is pressed (positive value)
      if (key != lastKey) {
        func(key, BtnState::PRESSED);
        lastKey = key;
      }
    } else if (key < 0) {         // key is released (negative value)
      uint8_t releasedKey = -key; // convert negative to positive key number
      if (releasedKey == lastKey) {
        func(releasedKey, BtnState::RELEASED);
        lastKey = 0; // reset lastKey after release
      }
    }
  }
};
