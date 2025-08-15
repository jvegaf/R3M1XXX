#pragma once

#include <Arduino.h>
#include <cstdint>

class Mux {
private:
  const uint8_t sig_pin;
  uint8_t position = 0;
  const uint8_t *mux_pins;                  // points to external array of select pins
  const uint8_t mux_pin_count = 4;                // number of select pins
  const uint8_t t_elements = 16;
  uint16_t p_state = 0;
  uint16_t c_state = 0;

  unsigned long lastdebouncetime = 0;
  unsigned long debouncedelay = 20;

  void setMuxChannel(uint8_t channel);

public:
  // Caller must ensure m_pins array remains valid for lifetime of Mux
  Mux(uint8_t sig, const uint8_t *m_pins)
      : sig_pin(sig), mux_pins(m_pins) {}

  void begin();
  void read(void (*up_func)(uint8_t), void (*down_func)(uint8_t));
};
