#include "BtnManager.h"
#include "LedManager.h"
#include "base.h"
#include <Arduino.h>
#include <ht16k33.h>

HT16K33 ht;
BtnManager btn_manager(ht);
LedManager led_manager(ht, 1, 8);

void logButtonEvent(uint8_t btnIndex, BtnState state) {
  Serial.print("Button ");
  Serial.print(btnIndex);
  Serial.print(state == BtnState::PRESSED ? " PRESSED" : " RELEASED");
  Serial.println();

  if (state == BtnState::PRESSED) {
    led_manager.toggle(btnIndex);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println("FX Pots Board Test");
  ht.begin(0x00);
  led_manager.allOn();
  delay(500);
  led_manager.allOff();
}

void loop() {
  btn_manager.update(logButtonEvent);
  delay(50);
}
