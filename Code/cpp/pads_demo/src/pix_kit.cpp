#include "pix_kit.h"

void PixKit::begin() {
  np_p->begin();
  np_p->setBrightness(30); // Set brightness to 50 out of 255
  np_p->show();            // Initialize all pixels to 'off'
}
void PixKit::setPixel(uint8_t n, uint32_t c) {
  if (n < num_pixels) {
    np_p->setPixelColor(n, c);
  }
}

void PixKit::show() { np_p->show(); }

uint32_t PixKit::Color(uint8_t r, uint8_t g, uint8_t b) {
  return np_p->Color(r, g, b);
}

uint8_t PixKit::numPixels() { return num_pixels; }

void PixKit::setRainbow() {
  for (uint8_t i = 0; i < num_pixels; i++) {
    uint8_t r = (i * 256 / num_pixels) & 0xFF;
    uint8_t g = (256 - i * 256 / num_pixels) & 0xFF;
    uint8_t b = 128; // Fixed blue component for a gradient effect
    np_p->setPixelColor(i, np_p->Color(r, g, b));
  }
  np_p->show();
}

void PixKit::clear() {
  np_p->clear();
  np_p->show();
}
