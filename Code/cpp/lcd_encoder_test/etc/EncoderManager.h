#pragma once
#include "Encoder.h"
#include "config.h"
#include <Wire.h>

// AIDEV-NOTE: EncoderManager handles 3 rotary encoders with buttons
// Each encoder has its own I2C address and MIDI note mapping
class EncoderManager {
private:
  struct EncoderComponent {
    Encoder enc;
    uint8_t ccw_note; // Note for counter-clockwise rotation
    uint8_t cw_note;  // Note for clockwise rotation
    uint8_t btn_note; // Note for button press

    // Constructor for EncoderComponent
    EncoderComponent(uint8_t addr, TwoWire &wire, uint8_t ccw, uint8_t cw,
                     uint8_t btn)
        : enc(addr, wire), ccw_note(ccw), cw_note(cw), btn_note(btn) {}
  };

  EncoderComponent *_encs;
  TwoWire *_wire;
  static const uint8_t NUM_ENCODERS = 3;
  // AIDEV-NOTE: Track previous button states to avoid sending duplicate MIDI messages
  BtnState _prevBtnStates[NUM_ENCODERS];

public:
  EncoderManager(TwoWire &w) : _wire(&w) {
    // AIDEV-NOTE: Initialize array of 3 EncoderComponent with their respective addresses and MIDI notes
    _encs = new EncoderComponent[NUM_ENCODERS]{
        EncoderComponent(ADDR_ENC_1, w, ENC_A_CCW_NOTE, ENC_A_CW_NOTE, ENC_A_BTN_NOTE),
        EncoderComponent(ADDR_ENC_2, w, ENC_B_CCW_NOTE, ENC_B_CW_NOTE, ENC_B_BTN_NOTE),
        EncoderComponent(ADDR_ENC_3, w, ENC_C_CCW_NOTE, ENC_C_CW_NOTE, ENC_C_BTN_NOTE)};
    
    // AIDEV-NOTE: Initialize all previous button states to IDLE
    for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
      _prevBtnStates[i] = BtnState::IDLE;
    }
  }

  ~EncoderManager() { delete[] _encs; }

  void begin() {
    for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
      _encs[i].enc.begin();
    }
  }

  void update(void (*func)(uint8_t, uint8_t, uint8_t)) {
    // AIDEV-NOTE: Update all encoders and send MIDI based on rotation and button
    for (uint8_t i = 0; i < NUM_ENCODERS; i++) {
      EncoderResponse response = _encs[i].enc.update();

      if (response.changed) {
        // AIDEV-NOTE: Handle rotation - send multiple MIDI notes based on delta
        if (response.direction != RotationDir::NONE) {
          uint8_t num_messages = abs(response.delta);
          uint8_t note_to_send;

          if (response.direction == RotationDir::CW) {
            note_to_send = _encs[i].cw_note;
          } else {
            note_to_send = _encs[i].ccw_note;
          }

          // AIDEV-NOTE: Send multiple MIDI note on/off pairs based on rotation speed
          for (uint8_t msg = 0; msg < num_messages; msg++) {
            func(note_to_send, 127U, MIDI_CH);
            func(note_to_send, 0, MIDI_CH);

            if (msg < num_messages - 1) {
              delayMicroseconds(ENCODER_MIDI_DELAY_US);
            }
          }
        }

        // AIDEV-NOTE: Handle button state changes
        // State machine: IDLE → PRESSED → RELEASED → IDLE
        if (response.state != _prevBtnStates[i]) {
          if (response.state == BtnState::PRESSED) {
            func(_encs[i].btn_note, 127U, MIDI_CH);
          } else if (response.state == BtnState::RELEASED) {
            func(_encs[i].btn_note, 0, MIDI_CH);
          }
          
          _prevBtnStates[i] = response.state;
        }
      }
    }
  }
};
