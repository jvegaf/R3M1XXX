#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <MIDI.h>
#include "config.h"

class EncoderManager {
private:
    uint8_t address;
    uint8_t ccRotation;
    uint8_t noteButton;
    
    int16_t lastValue;
    bool lastButtonState;
    bool initialized;

public:
    EncoderManager(uint8_t addr, uint8_t cc, uint8_t note) 
        : address(addr), ccRotation(cc), noteButton(note), 
          lastValue(0), lastButtonState(false), initialized(false) {}

    bool begin() {
        // Check if device is connected
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            initialized = true;
            
            // Initialize current state
            Wire.requestFrom(address, (uint8_t)3);
            if (Wire.available() == 3) {
                uint8_t low = Wire.read();
                uint8_t high = Wire.read();
                lastValue = (int16_t)(low | (high << 8));
                lastButtonState = (bool)Wire.read();
            }
            return true;
        }
        return false;
    }

    void update(midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>> &midi) {
        if (!initialized) return;

        // Request 3 bytes: Value (2) + Button (1)
        // According to Wagiminator docs: 
        // 1. Encoder wheel value (16-bit)
        // 2. Encoder switch state (8-bit)
        Wire.requestFrom(address, (uint8_t)3);
        
        if (Wire.available() == 3) {
            uint8_t low = Wire.read();
            uint8_t high = Wire.read();
            int16_t currentValue = (int16_t)(low | (high << 8));
            bool currentButtonState = (bool)Wire.read();

            // Handle Rotation
            if (currentValue != lastValue) {
                int16_t delta = currentValue - lastValue;
                
                // Limit delta to avoid MIDI flood in case of glitches
                if (delta > 10) delta = 10;
                if (delta < -10) delta = -10;

                // Send relative MIDI
                // 65 = +1 (CW), 63 = -1 (CCW)
                if (delta > 0) {
                    for(int i=0; i<delta; i++) midi.sendControlChange(ccRotation, 65, MIDI_CH);
                } else {
                    for(int i=0; i<abs(delta); i++) midi.sendControlChange(ccRotation, 63, MIDI_CH);
                }
                
                lastValue = currentValue;
            }

            // Handle Button
            if (currentButtonState != lastButtonState) {
                if (currentButtonState) {
                    midi.sendNoteOn(noteButton, 127, MIDI_CH);
                } else {
                    midi.sendNoteOff(noteButton, 0, MIDI_CH);
                }
                lastButtonState = currentButtonState;
            }
        }
    }
};
