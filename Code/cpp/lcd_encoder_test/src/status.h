#pragma once
#include "config.h"
#include <Arduino.h>

// AIDEV-NOTE: Status LED abstraction for different board types
// - BOARDPICO: Uses LED_BUILTIN (simple digital LED)
// - BOARDZERO: Uses WS2812 RGB NeoPixel on PIN_NEOPIXEL
// This allows the same API to control status indication across boards

#ifdef BOARDZERO
#include <Adafruit_NeoPixel.h>
#endif

namespace Status {

#ifdef BOARDZERO
// AIDEV-NOTE: Single NeoPixel for status on Waveshare RP2040 Zero
static Adafruit_NeoPixel statusPixel(1, PIN_RGB_BUILTIN, NEO_GRB + NEO_KHZ800);

// Default status color (green)
static uint8_t statusR = 0;
static uint8_t statusG = 32; // Dim green by default
static uint8_t statusB = 0;
#endif

inline void begin() {
#ifdef BOARDZERO
  statusPixel.begin();
  statusPixel.setBrightness(30); // Keep it dim to save power
  statusPixel.show();
#else
  // BOARDPICO or default: use built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
#endif
}

inline void setOn() {
#ifdef BOARDZERO
  statusPixel.setPixelColor(0, statusPixel.Color(statusR, statusG, statusB));
  statusPixel.show();
#else
  digitalWrite(LED_BUILTIN, HIGH);
#endif
}

inline void setOff() {
#ifdef BOARDZERO
  statusPixel.setPixelColor(0, 0);
  statusPixel.show();
#else
  digitalWrite(LED_BUILTIN, LOW);
#endif
}

inline void set(bool state) {
  if (state) {
    setOn();
  } else {
    setOff();
  }
}

#ifdef BOARDZERO
// AIDEV-NOTE: Extended API for RGB boards - set custom status color
inline void setColor(uint8_t r, uint8_t g, uint8_t b) {
  statusR = r;
  statusG = g;
  statusB = b;
}

// Set color and turn on in one call
inline void setColorOn(uint8_t r, uint8_t g, uint8_t b) {
  statusPixel.setPixelColor(0, statusPixel.Color(r, g, b));
  statusPixel.show();
}

// Convenience colors for status indication
inline void setError() { setColorOn(32, 0, 0); }    // Red
inline void setOk() { setColorOn(0, 32, 0); }       // Green
inline void setWarning() { setColorOn(32, 16, 0); } // Orange
inline void setBusy() { setColorOn(0, 0, 32); }     // Blue
#endif

} // namespace Status
