#include "EncoderTool.h"
#include <Arduino.h>
using namespace EncoderTool;

constexpr unsigned encoderCount = 8; // number of attached encoders

constexpr unsigned S0 = 5;    // address pin 0
constexpr unsigned S1 = 6;    //...
constexpr unsigned S2 = 7;    // address pin 2
constexpr unsigned SIG_A = 2; // output pin SIG of multiplexer A
constexpr unsigned SIG_B = 3;
// breakout:  https://www.sparkfun.com/products/13906
// datasheet: https://www.ti.com/lit/gpn/CD74HC4051

EncPlex4051 encoders(encoderCount, S0, S1, S2, SIG_A, SIG_B);

void onAnyEncoder(uint_fast8_t channel, int value, int delta);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  encoders.begin();
  encoders.attachCallback(onAnyEncoder);
}

void loop() { encoders.tick(); } // loop

void onAnyEncoder(uint_fast8_t channel, int value, int delta) {
  Serial.print("Encoder: ");
  Serial.print(channel);
  Serial.print(" value: ");
  Serial.print(value);
  Serial.print(" delta: ");
  Serial.print(delta);
  Serial.println();
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
