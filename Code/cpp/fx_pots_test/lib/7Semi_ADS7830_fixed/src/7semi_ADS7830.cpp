/*******************************************************************************
 * @file    7semi_ADS7830.cpp
 * @brief   Arduino Library for ADS7830 8-bit, 8-channel I2C ADC
 * @author  7semi
 * @date    2025
 *
 * @details
 *   - Supports single-ended readings from CH0–CH7
 *   - Supports differential readings using channel pairs
 *   - Optional internal 2.5V reference selection
 *
 * @usage
 *   #include "7semi_ADS7830.h"
 *   ADS7830_7semi adc(0x48);    // default I2C address
 *   adc.begin();
 *   uint8_t val = adc.readSingleCh(0); // read CH0
 *
 * @license MIT
 ******************************************************************************/

#include "7semi_ADS7830.h"

// AIDEV-NOTE: Definition of static const array moved from .h to .cpp
// to fix "extra qualification 'ADS7830_7semi::' on member 'SINGLE_ENDED_CH'" compilation error
const uint8_t ADS7830_7semi::SINGLE_ENDED_CH[8] = {
  0x80,  // CH0
  0xC0,  // CH1
  0x90,  // CH2
  0xD0,  // CH3
  0xA0,  // CH4
  0xE0,  // CH5
  0xB0,  // CH6
  0xF0   // CH7
};

/**
 * @brief Construct ADS7830 object with given I2C address
 * @param address I2C address of the ADS7830 (default = 0x48)
 */
ADS7830_7semi::ADS7830_7semi(uint8_t address) {
  i2c_address = address;
}

/**
 * @brief Initialize the ADS7830 with the specified I2C port
 * @param wirePort I2C port to use (default = Wire)
 * @return true if device responds, false otherwise
 */
bool ADS7830_7semi::begin(TwoWire &wirePort) {
  i2c = &wirePort;
  i2c->begin();
  i2c->beginTransmission(i2c_address);
  return (i2c->endTransmission() == 0);
}

/**
 * @brief Low-level read using a raw command byte
 * @param command Full control byte (includes SD/C2:C0)
 * @return 8-bit ADC result (0–255)
 */
uint8_t ADS7830_7semi::read(uint8_t command) {
  command |= int_ref; // Add internal ref bits (0x0C or 0x04)

  i2c->beginTransmission(i2c_address);
  i2c->write(command);
  if (i2c->endTransmission(false) != 0) return 0;

  i2c->requestFrom(i2c_address, (uint8_t)1);
  if (i2c->available()) {
    return i2c->read();
  }

  return 0;
}

/**
 * @brief Read single-ended input from CH0–CH7
 * @param channel Channel number (0–7)
 * @return 8-bit ADC value (0–255)
 */
uint8_t ADS7830_7semi::readSingleCh(uint8_t channel) {
  if (channel > 7) return 0;
  return read(SINGLE_ENDED_CH[channel]);
}

/**
 * @brief Read differential input using a DIFF_CHx_CHy macro
 * @param diffCommand One of the DIFF_CHx_CHy macros (0x00–0x70)
 * @return 8-bit ADC value (0–255)
 */
uint8_t ADS7830_7semi::readDiffCh(uint8_t diffCommand) {
  return read(diffCommand);
}

/**
 * @brief Enable or disable internal 2.5V reference mode
 * @param enable true = use internal ref (0x0C), false = default (0x04)
 * @return true (for chaining)
 */
bool ADS7830_7semi::intRef(bool enable) {
  int_ref = enable ? 0x0C : 0x04;
  return true;
}
