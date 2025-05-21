#pragma once

#include "Encoder.h"
#include <Arduino.h>
#include <Wire.h>

class Encoders {
public:
  Encoders(uint8_t t_enc, uint8_t *addrs, TwoWire &wire) : t_elements(t_enc), 
                                                              _addrs(addrs) {
    _i2cWire = &wire;
    _values = new uint16_t[t_enc]();
    _pressed = new boolean[t_enc]();
    _elements = new Encoder*[t_enc];
  }
  void begin();
  void checkState();

private:
  TwoWire *_i2cWire;
  uint8_t *_addrs;
  const uint8_t t_elements;
  uint16_t *_values;
  boolean *_pressed;
  Encoder **_elements;
};
