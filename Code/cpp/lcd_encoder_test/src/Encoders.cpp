#include "Encoders.h"

void Encoders::begin() {
  for (uint_8 i = 0; i < t_elements; i++) {
    _elements[i].begin;
  }
}

void Encoder::init(int16_t rmin, int16_t rmax, int16_t rstep, int16_t rval,
                   uint8_t rloop) {
  _i2cWire->beginTransmission(addr);
  _i2cWire->write((uint8_t)(rval & 0xff));
  _i2cWire->write((uint8_t)(rval >> 8));
  _i2cWire->write(0);
  _i2cWire->write(rloop);
  _i2cWire->write((uint8_t)(rmin & 0xff));
  _i2cWire->write((uint8_t)(rmin >> 8));
  _i2cWire->write((uint8_t)(rmax & 0xff));
  _i2cWire->write((uint8_t)(rmax >> 8));
  _i2cWire->write((uint8_t)(rstep & 0xff));
  _i2cWire->write((uint8_t)(rstep >> 8));
  _i2cWire->endTransmission();
}

bool Encoder::isPressed() {
  _i2cWire->requestFrom(addr, (uint8_t)3);
  _i2cWire->read();
  _i2cWire->read();
  boolean pressed = (_i2cWire->read());
  return pressed;
}

int16_t Encoder::getValue() {
  uint16_t pValue = 0;
  _i2cWire->requestFrom(addr, (uint8_t)2);
  if (_i2cWire->available()) {
    pValue = ((uint16_t)Wire.read() | ((uint16_t)Wire.read() << 8));
  }
  return pValue;
}

void Encoder::setValue(int16_t rval) {
  _i2cWire->beginTransmission(addr);
  _i2cWire->write((uint8_t)(rval & 0xff));
  _i2cWire->write((uint8_t)(rval >> 8));
  _i2cWire->endTransmission();
}
