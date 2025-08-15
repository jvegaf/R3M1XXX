#include "mux.h"

void Mux::setMuxChannel(uint8_t channel) {
  digitalWrite(mux_pins[0], bitRead(channel, 0));
  digitalWrite(mux_pins[1], bitRead(channel, 1));
  digitalWrite(mux_pins[2], bitRead(channel, 2));
  digitalWrite(mux_pins[3], bitRead(channel, 3));
}

void Mux::begin() {
  for (uint8_t i = 0; i < mux_pin_count; i++) {
    pinMode(mux_pins[i], OUTPUT);
    digitalWrite(mux_pins[i], LOW);
  }
  pinMode(sig_pin, INPUT_PULLUP);
}

void Mux::read(void (*up_func)(uint8_t), void (*down_func)(uint8_t)) {
  for (uint8_t i = 0; i < t_elements; i++) {
    setMuxChannel(i);
    c_state = digitalRead(sig_pin);

    if ((millis() - lastdebouncetime) > debouncedelay) {
      if (p_state != c_state) {
        lastdebouncetime = millis();

        if (c_state == HIGH) {
          up_func(i);
        } else {
          down_func(i);
        }

        p_state = c_state;
      }
    }
  }
}
