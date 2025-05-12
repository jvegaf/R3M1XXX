#pragma once

#include <Arduino.h>

class Encoder {
public:
  Encoder(uint8_t addr) : addr(addr), pValue(0), pPressed(false) {
    Encoder::init(0, 1, 1, 0, 1);
  }
  bool isPressed();
  int16_t getValue();

private:
  const uint8_t addr;
  uint16_t pValue;
  boolean pPressed;
  void init(int16_t rmin, int16_t rmax, int16_t rstep, int16_t rval,
            uint8_t rloop);
  void setValue(int16_t rval);
};
