/*******************************************************************************
 * @file    7semi_ADS7830.h
 * @brief   Arduino Library for ADS7830 8-bit, 8-channel I2C ADC
 * @author  7semi
 * @date    2025
 *
 * @details
 *   - Supports single-ended readings from CH0–CH7
 *   - Supports differential readings using channel pairs
 *   - Optional internal 2.5V reference selection
 *
 * @license MIT
 ******************************************************************************/
#ifndef _7SEMI_ADS7830_H_
#define _7SEMI_ADS7830_H_

#include <Arduino.h>
#include <Wire.h>

// Internal reference control
#define ENABLE  0x01
#define DISABLE 0x00

// Differential channel command codes
#define DIFF_CH0_CH1  0x00
#define DIFF_CH2_CH3  0x10
#define DIFF_CH4_CH5  0x20
#define DIFF_CH6_CH7  0x30
#define DIFF_CH1_CH0  0x40
#define DIFF_CH3_CH2  0x50
#define DIFF_CH5_CH4  0x60
#define DIFF_CH7_CH6  0x70

/**
 * @brief Class for ADS7830 8-bit I2C ADC
 */
class ADS7830_7semi {
public:
  ADS7830_7semi(uint8_t i2c_address = 0x48);
  bool begin(TwoWire &wirePort = Wire);
  bool intRef(bool enable); // true = internal reference
  uint8_t readSingleCh(uint8_t channel);
  uint8_t readSingle(uint8_t channel);
  uint8_t readDiffCh(uint8_t diffCommand);

private:
  TwoWire *i2c;
  uint8_t i2c_address;
  uint8_t int_ref = 0x04;         // default mode (internal ref disabled)
  uint8_t read(uint8_t command);  // low-level command read

  // AIDEV-NOTE: Static array declaration (definition moved to .cpp to fix compilation error)
  // Fix for: "extra qualification 'ADS7830_7semi::' on member 'SINGLE_ENDED_CH'"
  static const uint8_t SINGLE_ENDED_CH[8];
};

#endif
