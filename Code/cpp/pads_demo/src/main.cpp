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
  for (int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixel(i, pixels.Color(255, 0, 0)); // Red
    pixels.show();
    delay(100);
    pixels.clear();
    delay(100);
  }
}
