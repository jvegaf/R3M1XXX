#include "EncoderTool.h"
#include <Arduino.h>
using namespace EncoderTool;

constexpr unsigned encoderCount =
    3; // number of attached  (daisy chain shift regesters for more than 8)

constexpr unsigned QH_A = 10;  // output pin QH of shift register A
constexpr unsigned QH_B = 11;  // output pin QH of shift register B
constexpr unsigned pinLD = 12; // load pin for all shift registers)
constexpr unsigned pinCLK =
    13; // clock pin for all shift registers
        // 74165 datasheet: http://www.ti.com/product/SN74HC165

EncPlex74165 encoders(encoderCount, pinLD, pinCLK, QH_A, QH_B);

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
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Serial.begin(9600);
  // Serial.println("EncoderTool test");
  encoders.begin(CountMode::quarter);

  // encoders[0].attachCallback(onFirstEncoder); // standard callback
  encoders.attachCallback(onAnyEncoder);
}

void loop() { encoders.tick(); }
