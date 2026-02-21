#include "DisplayManager.h"
#include "config.h"
#include "status.h"
#include <Arduino.h>
#include <Wire.h>
#include <ht16k33.h>

#define PIN_I2C_SDA 5
#define PIN_I2C_SCL 6
#define PIN_RGB_BUILTIN 16

HT16K33 ht_dp;
DisplayManager dp_manager(ht_dp);

void setup() {
  Serial.begin(115200);
  Serial.println("I2C Encoder Test");
  Wire.setSDA(PIN_I2C_SDA);
  Wire.setSCL(PIN_I2C_SCL);
  Wire.begin();
  dp_manager.begin();
  Status::begin();
  Status::setOn();
  dp_manager.showDual(4, 32);
}

void loop() {
  for (int i = 0; i < 32; i++) {
    dp_manager.showDual(i, 32 - i);
    Serial.printf("Showing %d and %d\n", i, 32 - i);
    delay(100);
  }
}
