#pragma once

#include "Encoder.h"
#include <Arduino.h>
#include <Wire.h>

typedef enum {
  ENCODER_MODE_NORMAL = 0,
  ENCODER_MODE_INVERTED = 1,
} EncoderMode;

typedef enum {
  ENCODER_MODE_NONE = 0,
  ENCODER_MODE_DECREASE = 1,
  ENCODER_MODE_INCREASE = 2,
  ENCODER_MODE_PRESSED = 3,
} EncoderState;

class Encoders {
public:
  Encoders(uint8_t t_enc, uint8_t *addrs, TwoWire &wire) : t_elements(t_enc), 
                                                              _addrs(addrs) {
    _i2cWire = &wire;
    _pressed = new boolean[t_enc]();
    _elements = new Encoder*[t_enc];
    _mode = ENCODER_MODE_INVERTED;
  }
  void begin();
  void checkState();

private:
  TwoWire *_i2cWire;
  uint8_t *_addrs;
  const uint8_t t_elements;
  boolean *_pressed;
  Encoder **_elements;
  EncoderMode _mode;
  void parseState(EncoderState state);

};
