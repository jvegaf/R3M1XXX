#include "mux.h"
#include "pix_kit.h"
#include "pins.h"
#include <Arduino.h>

Mux mux_pad(MUX_SIG, (uint8_t[]){MUX_S0, MUX_S1, MUX_S2, MUX_S3});
PixKit pixels(PIXEL_PIN, 16);


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println("Hello, World!");
  mux_pad.begin();
  Serial.println("muxpad initializated");
  pixels.begin();
  Serial.println("pixels initializated");
  pixels.setRainbow();
  Serial.println("pixels rainbow set");
  delay(1000);
  pixels.clear();
  Serial.println("pixels cleared");
}

void loop() {
  mux_pad.read([](uint8_t i) { Serial.printf("Pad %d released\n", i); },
               [](uint8_t i) { Serial.printf("Pad %d pressed\n", i); });

  // put your main code here, to run repeatedly:
}
