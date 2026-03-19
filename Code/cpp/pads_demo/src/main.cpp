#include "base.h"
#include "pix_kit.h"
#include <Arduino.h>

PixKit pixels(PIXEL_PIN, NUM_PIXELS);

void setup() {
  pixels.begin();
  Serial.begin(9600);
  Serial.println("Hello, World!");
  Serial.println("pixels initializated");
  // while (!Serial) {
  //   ; // wait for serial port to connect. Needed for native USB
  // }
}

void loop() {
  pixels.clear();
  delay(1000);
  pixels.setRainbow();
  delay(1000);
}
