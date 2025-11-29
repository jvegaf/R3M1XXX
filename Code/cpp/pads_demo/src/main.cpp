#include "pins.h"
#include "pix_kit.h"
#include <Arduino.h>

PixKit pixels(PIXEL_PIN, 16);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println("Hello, World!");
  pixels.begin();
  Serial.println("pixels initializated");
}

void loop() {
  pixels.setRainbow();
  Serial.println("pixels rainbow set");
  // delay(1000);
  // pixels.clear();
  // Serial.println("pixels cleared");
  delay(1000);
}
