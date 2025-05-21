#include "Encoders.h"

void Encoders::begin() {
  for (uint8_t i = 0; i < t_elements; i++) {
    _elements[i] = new Encoder(_addrs[i], _i2cWire);
    _elements[i]->begin();
    _elements[i]->setValue(0);
  }
}

void Encoders::checkState() {
  for (uint8_t i = 0; i < t_elements; i++) {
    int16_t value = _elements[i]->getValue();
    boolean pressed = _elements[i]->isPressed();
    if (value != 0) {
      Serial.print("Encoder ");
      Serial.print(i);
      if (_mode == ENCODER_MODE_INVERTED) {
        value = -value;
      }
      if (value > 0) {
        parseState(ENCODER_MODE_INCREASE);
      } else if (value < 0) {
        parseState(ENCODER_MODE_DECREASE);
      }
      _elements[i]->setValue(0);
    }
    if (_pressed[i] != pressed) {
      if (_pressed[i]) {
        parseState(ENCODER_MODE_PRESSED);
      } else {
        parseState(ENCODER_MODE_NONE);
      }
      _pressed[i] = pressed;
    }
  }
}

void Encoders::parseState(EncoderState state) {
  switch (state) {
  case ENCODER_MODE_DECREASE:
    Serial.println(" Decrease");
    break;
  case ENCODER_MODE_INCREASE:
    Serial.println(" Increase");
    break;
  case ENCODER_MODE_PRESSED:
    Serial.println(" Pressed");
    break;
  default:
    break;
  }
}