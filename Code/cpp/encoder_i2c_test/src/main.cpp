#include "Encoder.h"
#include <Arduino.h>

// #define encoder_addr 0x38 // 0x32 or 0x36 or 0x38

byte enc_addrs[] = {0x32, 0x34, 0x36};
int16_t pValues[] = {0, 0, 0};
boolean pPressed[] = {false, false, false};

Encoder encoders[] = {Encoder(enc_addrs[0], Wire), Encoder(enc_addrs[1], Wire),
                      Encoder(enc_addrs[2], Wire)};

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("I2C Encoder Test");
  Wire.begin();
  for (int i = 0; i < 3; i++) {
    encoders[i].begin();
    encoders[i].setValue(0);
    Serial.print("Encoder ");
    Serial.print(i);
    Serial.print(" address: ");
    Serial.println(enc_addrs[i], HEX);
  }
  Serial.println("Encoder set");
}

void loop() {
  for (int i = 0; i < 3; i++) {
    int16_t value = encoders[i].getValue();
    boolean pressed = encoders[i].isPressed();
    if (pValues[i] != value) {
      pValues[i] = value;
      Serial.print("Encoder ");
      Serial.print(i);
      Serial.println(" value changed");
      if (pValues[i] > 30) {
        pValues[i] = 0;
        encoders[i].setValue(pValues[i]);
      } else if (pValues[i] < -30) {
        pValues[i] = 0;
        encoders[i].setValue(pValues[i]);
      }
      Serial.print("Value: ");
      Serial.println(pValues[i]);
    }
    if (pPressed[i] != pressed) {
      if (pPressed[i]) {
        Serial.print("Encoder ");
        Serial.print(i);
        Serial.println(" switch was pressed");
      }
      pPressed[i] = pressed;
    }
  }
  delay(20);
}
