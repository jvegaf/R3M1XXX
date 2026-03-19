#include "pix_kit.h"
#include "base.h"

void PixKit::begin() {
  np_p->begin();
  np_p->setBrightness(30); // Set brightness to 50 out of 255
  np_p->show();            // Initialize all pixels to 'off'
  for (int i = 0; i < NUM_PIXELS; i++) {
    np_p->setPixelColor(pix_pos[i] - 1, 255, 0, 0);
    np_p->show();
    delay(50);
  }
  np_p->clear();
}
void PixKit::setPixel(uint8_t n, uint32_t c) {
  if (n < num_pixels) {
    np_p->setPixelColor(pix_pos[n] - 1, c);
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
