#pragma once

#include <Arduino.h>

#define PIXEL_PIN 6
#define PIN_I2C_SDA 4
#define PIN_I2C_SCL 5
#define NUM_PIXELS 16
#define HT16K33_ADDRESS 0x01
#define READ_INTERVAL_MS 20

// Performance optimization constants
#define MIDI_BAUD_RATE 31250
#define MIDI_CHANNEL 3
#define MIDI_NOTE_MIN 36
#define MIDI_BAUD_RATE 31250
#define MAX_MIDI_VELOCITY 127
#define MIN_MIDI_VELOCITY 0

// Error handling
#define MIDI_TIMEOUT_MS 1000

const uint8_t pix_idx[] = {1, 2,  3,  4,  8,  7,  6,  5,
                           9, 10, 11, 12, 16, 15, 14, 13};

const uint8_t btn_idx[] = {1,  2,  3,  4,  5,  6,  7,  8,
                           14, 18, 15, 16, 17, 19, 20, 21};

inline uint8_t getBtnIndex(uint8_t btnNum) {
  for (uint8_t i = 0; i < NUM_PIXELS; i++) {
    if (btn_idx[i] == btnNum) {
      return i;
    }
  }
  return 0;
}
