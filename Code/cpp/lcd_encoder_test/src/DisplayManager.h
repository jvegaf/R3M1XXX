#pragma once
#include "config.h"
#include <ht16k33.h>
#include <Wire.h>

class DisplayManager {
private:
  HT16K33 &ht;
  uint8_t brightness = DISPLAY_BRIGHTNESS;

public:
  DisplayManager(HT16K33 &ic) : ht(ic) {}

  // 7-segment font: A=0x01, B=0x02, C=0x04, D=0x08, E=0x10, F=0x20, G=0x40, DP=0x80
  const uint8_t font[10] = {
      0x3F, // 0
      0x06, // 1
      0x5B, // 2
      0x4F, // 3
      0x66, // 4
      0x6D, // 5
      0x7D, // 6
      0x07, // 7
      0x7F, // 8
      0x6F  // 9
  };

  // AIDEV-NOTE: Digit mapping to HT16K33 display RAM
  // Digit 0-2 (Left display): COM1-COM3 -> Address 0x02, 0x04, 0x06
  // Digit 3-5 (Right display): COM4-COM6 -> Address 0x08, 0x0A, 0x0C
  const uint8_t digitMap[6] = {2, 4, 6, 8, 10, 12};

  void begin() {
    // AIDEV-NOTE: Chip is already initialized by BtnManager with keyscan mode
    // We only clear display RAM and set brightness using direct I2C
    clearAllDisplay();
    setBrightnessDirect(brightness);
  }
  
private:
  void clearAllDisplay() {
    Wire.beginTransmission(ADDR_DISPLAY | 0x70);
    Wire.write(0x00);
    for (int i = 0; i < 16; i++) {
      Wire.write(0x00);
    }
    Wire.endTransmission();
  }
  
  void setBrightnessDirect(uint8_t level) {
    if (level > 15) level = 15;
    Wire.beginTransmission(ADDR_DISPLAY | 0x70);
    Wire.write(0xE0 | level);
    Wire.endTransmission();
  }

public:

  void showBPM(uint16_t bpm) {
    uint8_t hundreds = (bpm / 100) % 10;
    uint8_t tens = (bpm / 10) % 10;
    uint8_t ones = bpm % 10;

    // Left Display
    if (bpm >= 100) {
      drawDigit(0, hundreds);
    } else {
      clearDigit(0);
    }
    drawDigit(1, tens);
    drawDigit(2, ones);

    // Right Display
    if (bpm >= 100) {
      drawDigit(3, hundreds);
    } else {
      clearDigit(3);
    }
    drawDigit(4, tens);
    drawDigit(5, ones);
  }

  void showDual(uint16_t leftVal, uint16_t rightVal) {
    // Left Display
    uint8_t l_hundreds = (leftVal / 100) % 10;
    uint8_t l_tens = (leftVal / 10) % 10;
    uint8_t l_ones = leftVal % 10;

    if (leftVal >= 100)
      drawDigit(0, l_hundreds);
    else
      clearDigit(0);
    if (leftVal >= 10)
      drawDigit(1, l_tens);
    else
      clearDigit(1);
    drawDigit(2, l_ones);

    // Right Display
    uint8_t r_hundreds = (rightVal / 100) % 10;
    uint8_t r_tens = (rightVal / 10) % 10;
    uint8_t r_ones = rightVal % 10;

    if (rightVal >= 100)
      drawDigit(3, r_hundreds);
    else
      clearDigit(3);
    if (rightVal >= 10)
      drawDigit(4, r_tens);
    else
      clearDigit(4);
    drawDigit(5, r_ones);
  }

  void setDigit(uint8_t digit, uint8_t value, bool dp = false) {
    if (digit < 6) {
      uint8_t bufferIndex = digitMap[digit];
      if (dp)
        value |= 0x80;
      writeDisplayRaw(bufferIndex, value);
    }
  }

  void update() { 
    // No-op when using direct I2C writes
  }

  void showError(uint8_t code) {
    writeDisplayRaw(digitMap[0], 0x79); // E
    writeDisplayRaw(digitMap[1], 0x50); // r
    writeDisplayRaw(digitMap[2], 0x50); // r

    uint8_t hundreds = (code / 100) % 10;
    uint8_t tens = (code / 10) % 10;
    uint8_t ones = code % 10;

    drawDigit(3, hundreds);
    drawDigit(4, tens);
    drawDigit(5, ones);
  }

private:
  void drawDigit(uint8_t digitIndex, uint8_t number) {
    if (digitIndex >= 6 || number > 9)
      return;

    uint8_t pattern = font[number];
    uint8_t bufferIndex = digitMap[digitIndex];
    writeDisplayRaw(bufferIndex, pattern);
  }

  void clearDigit(uint8_t digitIndex) {
    if (digitIndex >= 6)
      return;
    uint8_t bufferIndex = digitMap[digitIndex];
    writeDisplayRaw(bufferIndex, 0);
  }
  
  // AIDEV-NOTE: Write directly to display RAM without disturbing keyscan configuration
  void writeDisplayRaw(uint8_t address, uint8_t data) {
    Wire.beginTransmission(ADDR_DISPLAY | 0x70);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
  }
};
