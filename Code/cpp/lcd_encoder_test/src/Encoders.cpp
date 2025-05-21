#include "Encoders.h"

void Encoders::begin() {
  for (uint8_t i = 0; i < t_elements; i++) {
    _elements[i] = new Encoder(_addrs[i], _i2cWire);
    _elements[i]->begin();
  }
}

void Encoders::checkState() {
  for (uint8_t i = 0; i < t_elements; i++) {
    int16_t value = _elements[i]->getValue();
    boolean pressed = _elements[i]->isPressed();
    if (_values[i] != value) {
      _values[i] = value;
      Serial.print("Encoder ");
      Serial.print(i);
      Serial.println(" value changed");
      if (_values[i] > 30) {
        _values[i] = 0;
        _elements[i]->setValue(_values[i]);
      } else if (_values[i] < -30) {
        _values[i] = 0;
        _elements[i]->setValue(_values[i]);
      }
      Serial.print("Value: ");
      Serial.println(_values[i]);
    }
    if (_pressed[i] != pressed) {
      if (_pressed[i]) {
        Serial.print("Encoder ");
        Serial.print(i);
        Serial.println(" switch was pressed");
      }
      _pressed[i] = pressed;
    }
  }
}