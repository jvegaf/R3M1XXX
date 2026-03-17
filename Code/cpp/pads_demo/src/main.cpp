#include "pins.h"
#include "pix_kit.h"
#include <Arduino.h>

PixKit pixels(PIXEL_PIN, 16);

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
  pixels.setRainbow();
  Serial.println("pixels rainbow set");
  delay(1000);
  pixels.clear();
  Serial.println("pixels cleared");
  delay(1000);
}
