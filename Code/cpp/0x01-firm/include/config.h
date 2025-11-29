#pragma once
#include <Arduino.h>

// --- HARDWARE PINS (RP2040) ---
#define PIN_I2C_SDA 4
#define PIN_I2C_SCL 5
#define PIN_NEOPIXEL 16

// --- I2C ADDRESSES ---
#define ADDR_POTS 0x48
#define ADDR_MATRIX_MAIN 0x70 // 8 botones + 8 leds
#define ADDR_DISPLAY 0x71     // 7 segmentos
#define ADDR_MATRIX_AUX 0x72  // 16 botones
#define ADDR_ENC_1 0x30       // Encoder 1
#define ADDR_ENC_2 0x31       // Encoder 2
#define ADDR_ENC_3 0x32       // Encoder 3

// --- MIDI SETTINGS ---
#define MIDI_CH 1
#define CC_POTS_START 10   // Los pots serán CC 10 al 17
#define CC_ENC_START 20    // Encoders CC 20, 21, 22
#define NOTE_MAIN_START 36 // C1
#define NOTE_AUX_START 52  // E2
#define NOTE_ENC_START 68  // Notas para pulsadores de encoder

// --- LED SETTINGS ---
#define NUM_PIXELS 16
#define LED_BRIGHTNESS 50 // 0-255 (Ahorra energía del USB)
