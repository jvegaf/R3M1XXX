
#pragma once
#include <Arduino.h>

#define ADDR_FX_BTNS 0x00 // Base addr -> becomes 0x70: 8 botones + 8 leds

// AIDEV-NOTE: FX Buttons mapping (8 buttons detected on 0x70)
// Format: {COM, ROW} - Values from hardware scan
// Order can be rearranged after testing to match physical layout
#ifndef BTN_FX_POSITIONS
static const uint8_t BTN_FX_POSITIONS[][2] = {
    {0, 0}, // FX Button 7: COM1 ROW8 → MIDI Note 43
    {0, 1}, // FX Button 0: COM0 ROW1 → MIDI Note 36
    {0, 2}, // FX Button 1: COM0 ROW2 → MIDI Note 37
    {0, 3}, // FX Button 2: COM0 ROW3 → MIDI Note 38
    {0, 4}, // FX Button 3: COM0 ROW4 → MIDI Note 39
    {0, 5}, // FX Button 4: COM1 ROW5 → MIDI Note 40
    {0, 6}, // FX Button 5: COM1 ROW6 → MIDI Note 41
    {0, 7}, // FX Button 6: COM1 ROW7 → MIDI Note 42
};
#define NUM_FX_BTNS 8
#endif
