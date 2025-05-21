#include "Encoders.h"
#include <Arduino.h>
#include <Wire.h>

byte enc_addrs[] = {0x32, 0x36, 0x38};

Encoders encoders(3, enc_addrs, Wire);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("I2C Encoder Test");
  Wire.begin();
  encoders.begin();
  Serial.println("Encoders set");
}

void loop() {
  encoders.checkState();
  delay(20);
}
