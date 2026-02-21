#pragma once

#include "base.h"
#include <Arduino.h>
#include <Wire.h>

class Encoder {
public:
  Encoder(uint8_t addr, TwoWire &wire) : addr(addr), _lastButtonChangeMs(0) { 
    _i2cWire = &wire; 
  }
  void begin();
  EncoderResponse update();

private:
  const uint8_t addr;
  TwoWire *_i2cWire;
  BtnState _pState;
  int16_t _pValue;
  // AIDEV-NOTE: Timestamp for button debounce to prevent multiple MIDI messages
  // from a single button press/release event
  unsigned long _lastButtonChangeMs;

  void init(int16_t rmin, int16_t rmax, int16_t rstep, int16_t rval,
            uint8_t rloop);

  BtnState getState(BtnState pState);
  int16_t getValue();
};
