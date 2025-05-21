#pragma once

#include "Encoder.h"
#include <Arduino.h>
#include <Wire.h>

class Encoders {
public:
  Encoders(uint8_t t_enc, uint8_t *addrs, TwoWire &wire) : t_elements(t_enc) {
    _elements = new uint8_t[t_enc]();
    _values = new uint16_t[t_enc]();
    _pressed = new boolean[t_enc]();
    for (uint8_t i = 0; i < t_enc; i++) {
      _elements[i] = Encoder(addrs[i], &wire);
    }
  }
  void begin();
  void checkState();

private:
  const uint8_t t_elements;
  const uint16_t *_values;
  const boolean *_pressed;
  const uint8_t *_elements;
  bool isPressed();
  int16_t getValue();
  void setValue(int16_t rval);
};
