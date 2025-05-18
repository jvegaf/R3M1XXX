#pragma once

#include <Arduino.h>
#include <Wire.h>

class Encoder {
public:
  Encoder(uint8_t addr, TwoWire &wire) : addr(addr) { _i2cWire = &wire; }
  void begin();
  bool isPressed();
  int16_t getValue();
  void setValue(int16_t rval);

private:
  const uint8_t addr;
  TwoWire *_i2cWire;
  void init(int16_t rmin, int16_t rmax, int16_t rstep, int16_t rval,
            uint8_t rloop);
};
