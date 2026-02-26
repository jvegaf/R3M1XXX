#pragma once
#include <ht16k33.h>

// AIDEV-NOTE: LedManager controls 8 LEDs connected to HT16K33 at 0x70
// Uses direct I2C writes to avoid disturbing keyscan configuration set by
// BtnManager
class LedManager {
private:
  HT16K33 &ht;
  uint8_t first_led;
  uint8_t total_leds;

public:
  LedManager(HT16K33 &ic, uint8_t first_led, uint8_t total_leds)
      : ht(ic), first_led(first_led), total_leds(total_leds) {}

  void on(uint8_t ledIndex) { ht.setLedNow(ledIndex + first_led); }

  void off(uint8_t ledIndex) { ht.clearLedNow(ledIndex + first_led); }

  void toggle(uint8_t ledIndex) {
    uint8_t ledNo = ledIndex + first_led;
    if (ht.getLed(ledNo)) {
      ht.clearLedNow(ledNo);
    } else {
      ht.setLedNow(ledNo);
    }
  }

  void allOff() {
    for (uint8_t led = first_led; led < total_leds + first_led; led++) {
      ht.clearLedNow(led);
      delay(10);
    }
  }

  void allOn() {
    for (uint8_t led = first_led; led < total_leds + first_led; led++) {
      ht.setLedNow(led);
      delay(10);
    }
  }
};
