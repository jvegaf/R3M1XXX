#include "Encoder.h"
#include "config.h"

void Encoder::begin() { 
  // AIDEV-NOTE: Initialize state to IDLE to prevent garbage values
  _pState = BtnState::IDLE;
  _pValue = 0;
  init(-50, 50, 1, 0, 1); 
}

EncoderResponse Encoder::update() {
  bool changed = false;
  BtnState state = this->getState(this->_pState);
  
  // AIDEV-NOTE: Debounce logic - only apply debounce to transitions FROM IDLE
  if (this->_pState != state) {
    // Check if this is a transition FROM IDLE (needs debounce)
    if (this->_pState == BtnState::IDLE && state != BtnState::IDLE) {
      unsigned long currentMs = millis();
      // Only register the change if debounce time has elapsed
      if (currentMs - this->_lastButtonChangeMs >= ENCODER_BUTTON_DEBOUNCE_MS) {
        changed = true;
        this->_lastButtonChangeMs = currentMs;
      } else {
        // Ignore this state change - it's too soon (bounce)
        state = this->_pState;
      }
    } else {
      // All other transitions (PRESSED→RELEASED, RELEASED→IDLE) happen immediately
      changed = true;
    }
  }
  this->_pState = state;
  
  // AIDEV-NOTE: Calculate value change and rotation direction
  int16_t _cValue = this->getValue();
  int16_t delta = 0;
  RotationDir direction = RotationDir::NONE;
  
  if (this->_pValue != _cValue) {
    delta = _cValue - this->_pValue;
    
    // Determine rotation direction based on delta
    if (delta > 0) {
      direction = RotationDir::CW;
    } else if (delta < 0) {
      direction = RotationDir::CCW;
    }
    
    this->_pValue = _cValue;
    changed = true;
  }
  
  return {changed, this->_pValue, this->_pState, direction, delta};
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

BtnState Encoder::getState(BtnState pState) {
  int bytesReceived = _i2cWire->requestFrom(addr, (uint8_t)3);
  
  // AIDEV-NOTE: Return previous state on I2C error
  if (bytesReceived != 3) {
    return pState;
  }
  
  _i2cWire->read(); // byte 0 (value LSB)
  _i2cWire->read(); // byte 1 (value MSB)
  uint8_t btnByte = _i2cWire->read(); // byte 2 (button state)
  
  // AIDEV-NOTE: Button pressed when btnByte is 0 (active low)
  bool pressed = (btnByte == 0);
  
  // AIDEV-NOTE: State machine for button press/release detection
  // IDLE → PRESSED: Button just pressed
  if (pressed && pState == BtnState::IDLE) {
    return BtnState::PRESSED;
  }
  
  // PRESSED → RELEASED: Button just released
  if (!pressed && pState == BtnState::PRESSED) {
    return BtnState::RELEASED;
  }
  
  // RELEASED → IDLE: Complete the release cycle
  if (!pressed && pState == BtnState::RELEASED) {
    return BtnState::IDLE;
  }
  
  // No change - maintain current state
  return pState;
}

int16_t Encoder::getValue() {
  uint16_t pValue = 0;
  _i2cWire->requestFrom(addr, (uint8_t)2);
  if (_i2cWire->available() >= 2) {
    pValue = ((uint16_t)_i2cWire->read() | ((uint16_t)_i2cWire->read() << 8));
  }
  return pValue;
}
