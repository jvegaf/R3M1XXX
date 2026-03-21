#pragma once

#include <Arduino.h>

#define PIXEL_PIN 4
#define NUM_PIXELS 16
#define HT16K33_ADDRESS 0x01
#define READ_INTERVAL_MS 20

const uint8_t pix_pos[] = {1, 2,  3,  4,  8,  7,  6,  5,
                           9, 10, 11, 12, 16, 15, 14, 13};
