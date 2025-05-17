#include "Encoder.h"
#include <Arduino.h>

// #define encoder_addr 0x38 // 0x32 or 0x36 or 0x38

byte encoder_addr = 0x38; // I2C address of the encoder

int16_t value, lastvalue;
boolean pressed, lastpressed;

Encoder encoder(encoder_addr, Wire);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("I2C Encoder Test");
  Wire.begin();
  encoder.begin();
  Serial.println("Encoder set");
}

void loop() {
  value = encoder.getValue();
  if (value != lastvalue) {
    Serial.println("Value changed");
    Serial.println(value);
    if (value > 30) {
      value = 0;
      encoder.setValue(value);
    } else if (value < -30) {
      value = 0;
      encoder.setValue(value);
    }
    lastvalue = value;
  }
  pressed = encoder.isPressed();
  if (pressed != lastpressed) {
    if (pressed)
      Serial.println("Switch was pressed");
    lastpressed = pressed;
  }
  dkqqelay(20);
}
