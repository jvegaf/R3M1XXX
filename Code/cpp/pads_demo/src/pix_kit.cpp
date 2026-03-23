#include "pix_kit.h"
#include "base.h"

const uint32_t CLEAR_COL = Adafruit_NeoPixel::Color(0, 0, 0);
const uint32_t BLUE_COL = Adafruit_NeoPixel::Color(0, 0, 255);
const uint32_t GREEN_COL = Adafruit_NeoPixel::Color(0, 255, 0);
const uint32_t RED_COL = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t YELLOW_COL = Adafruit_NeoPixel::Color(255, 240, 0);
const uint32_t ORANGE_COL = Adafruit_NeoPixel::Color(255, 133, 0);
const uint32_t PURPLE_COL = Adafruit_NeoPixel::Color(243, 0, 255);
const uint32_t GRAY_COL = Adafruit_NeoPixel::Color(120, 120, 120);

static const uint32_t Colors[] = {
    CLEAR_COL,  // 0: no hotcue
    BLUE_COL,   // 1: cue
    ORANGE_COL, // 2: fade out
    YELLOW_COL, // 3: load
    GRAY_COL,   // 4: grid
    GREEN_COL,  // 5: loop
    RED_COL,    // 6: additional
    PURPLE_COL  // 7: additional
};

const uint8_t tColors = 8;

void PixKit::begin() {
  np_p->begin();
  np_p->setBrightness(30); // Set brightness to 50 out of 255
  np_p->show();            // Initialize all pixels to 'off'
  for (int i = 0; i < NUM_PIXELS; i++) {
    // np_p->setPixelColor(pix_pos[i] - 1, 255, 0, 0);
    np_p->setPixelColor(pix_idx[i] - 1, Colors[4]);
    np_p->show();
    delay(50);
  }
  np_p->clear();
  np_p->show();
}
void PixKit::setPixel(uint8_t n, uint8_t c) {
  if (n < num_pixels) {
    if (c >= tColors) {
      c = tColors - 1;
    }
    np_p->setPixelColor(pix_idx[n] - 1, Colors[c]);
    np_p->show();
  }
}

void PixKit::setAll(uint8_t c) {
  if (c >= tColors) {
    c = tColors - 1;
  }
  for (uint8_t i = 0; i < num_pixels; i++) {
    np_p->setPixelColor(pix_idx[i] - 1, Colors[c]);
  }

  np_p->show();
}

uint32_t PixKit::Color(uint8_t r, uint8_t g, uint8_t b) {
  return np_p->Color(r, g, b);
}

void PixKit::setRainbow() {
  for (uint8_t i = 0; i < num_pixels; i++) {
    // uint8_t r = (i * 256 / num_pixels) & 0xFF;
    // uint8_t g = (256 - i * 256 / num_pixels) & 0xFF;
    // uint8_t b = 128; // Fixed blue component for a gradient effect
    // np_p->setPixelColor(i, np_p->Color(r, g, b));
    uint8_t col = i;
    if (i >= tColors) {
      col = i - tColors;
    }
    np_p->setPixelColor(i, Colors[col]);
  }
  np_p->show();
}

void PixKit::clear() {
  np_p->clear();
  np_p->show();
}
