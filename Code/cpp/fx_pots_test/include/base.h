#pragma once
#include <Arduino.h>

enum class BtnState { PRESSED, RELEASED, IDLE };

// AIDEV-NOTE: Direction enum for encoder rotation
enum class RotationDir { CW, CCW, NONE };

struct EncoderResponse {
  bool changed;
  int16_t enc_value;
  BtnState state;
  // AIDEV-NOTE: Added rotation direction and delta for velocity-sensitive MIDI
  RotationDir direction;
  int16_t delta; // Difference from previous value (velocity of rotation)
};
