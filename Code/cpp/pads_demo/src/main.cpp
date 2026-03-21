#include "base.h"
#include "pix_kit.h"
#include <Arduino.h>
#include "ht16k33.h"

PixKit pixels(PIXEL_PIN, NUM_PIXELS);
HT16K33 HT;

volatile unsigned long lastRead = 0;
volatile uint8_t lastKey = 0;

void readPad() {
  int8_t key;
  key = HT.readKey();
  if (key != 0) { // key is pressed
    if (key != lastKey) {
      Serial.print(F("Key pressed: "));
      Serial.println(key);
      lastKey = key;
    }
  }
  lastRead = millis();
}

void setup() {
  Serial.begin(115200);
  pixels.begin();
  HT.begin(HT16K33_ADDRESS);
  lastRead = millis();
  Serial.println("Begin");
}

void loop() {
  unsigned long currentTime = millis();
  pixels.setRainbow();
  if (lastRead + READ_INTERVAL_MS < currentTime ) {
    readPad();
  }
}
