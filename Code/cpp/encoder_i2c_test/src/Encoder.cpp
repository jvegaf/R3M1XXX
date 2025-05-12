#include "Encoder.h"
#include "Arduino.h"
#include <Wire.h>

void Encoder::init(int16_t rmin, int16_t rmax, int16_t rstep, int16_t rval,
                   uint8_t rloop) {
  Wire.begin();
  Wire.beginTransmission(addr);
  Wire.write((uint8_t)(rval & 0xff));
  Wire.write((uint8_t)(rval >> 8));
  Wire.write(0);
  Wire.write(rloop);
  Wire.write((uint8_t)(rmin & 0xff));
  Wire.write((uint8_t)(rmin >> 8));
  Wire.write((uint8_t)(rmax & 0xff));
  Wire.write((uint8_t)(rmax >> 8));
  Wire.write((uint8_t)(rstep & 0xff));
  Wire.write((uint8_t)(rstep >> 8));
  Wire.endTransmission();
}

bool Encoder::isPressed() {
  Wire.requestFrom(addr, 3);
  Wire.read();
  Wire.read();
  boolean cPressed = (Wire.read());
  if (cPressed != pPressed) {
    pPressed = cPressed;
  }
  return pPressed;
}

int16_t Encoder::getValue() {
  Wire.requestFrom(addr, 2);
  if (Wire.available()) {
    uint16_t cValue = ((uint16_t)Wire.read() | ((uint16_t)Wire.read() << 8));
    if (cValue != pValue) {
      pValue = cValue;
    }
  }
  return pValue;
}

void Encoder::setValue(int16_t rval) {
  Wire.beginTransmission(addr);
  Wire.write((uint8_t)(rval & 0xff));
  Wire.write((uint8_t)(rval >> 8));
  Wire.endTransmission();
}
