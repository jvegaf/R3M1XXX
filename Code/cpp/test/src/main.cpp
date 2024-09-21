#include "EncoderTool.h"
#include <Arduino.h>
using namespace EncoderTool;

uint_fast8_t encoderCount = 8; // number of attached encoders

uint_fast8_t  S0 = 6;     // address pin 0
uint_fast8_t  S1 = 7;     //...
uint_fast8_t  S2 = 8;     // address pin 2
uint_fast8_t  SIG_A = 10; // output pin SIG of multiplexer A
uint_fast8_t  SIG_B = 11;
// breakout:  https://www.sparkfun.com/products/13906
// datasheet: https://www.ti.com/lit/gpn/CD74HC4051

EncPlex4051 encoders(encoderCount, S0, S1, S2, SIG_A, SIG_B);

void onFirstEncoder(int value, int delta) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void onAnyEncoder(uint_fast8_t channel, int value, int delta) {
  Serial.print("Encoder: ");
  Serial.print(channel);
  Serial.print(" value: ");
  Serial.print(value);
  Serial.print(" delta: ");
  Serial.print(delta);
  Serial.println();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("EncoderTool test");
  encoders.begin(CountMode::quarter);

  encoders[0].attachCallback(onFirstEncoder); // standard callback
  encoders.attachCallback(onAnyEncoder);
}

void loop() { encoders.tick(); }
