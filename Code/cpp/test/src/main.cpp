/****************************************************************
 * Simple test of ht16k33 library both led and keyboard
 */
#include <Arduino.h>
#include "ht16k33.h"
#include "EncoderTool.h"
using namespace EncoderTool;

constexpr unsigned encoderCount = 8;  // number of attached encoders

constexpr unsigned S0 = 18;            //address pin 0
constexpr unsigned S1 = 19;            //...
constexpr unsigned S2 = 20;            //address pin 2
constexpr unsigned SIG_A = 21;         //output pin SIG of multiplexer A
constexpr unsigned SIG_B = 22;         //output pin SIG of multiplexer B
                                      //breakout:  https://www.sparkfun.com/products/13906
                                      //datasheet: https://www.ti.com/lit/gpn/CD74HC4051

EncPlex4051 encoders(encoderCount, S0, S1, S2, SIG_A, SIG_B);
HT16K33 HT;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  encoders.begin();
  Serial.begin(9600);
  Serial.println("ht16k33 keyboard and led test v0.01");
  Serial.println();
  // initialize everything, 0x00 is the i2c address for the first chip (0x70 is added in the class).
  HT.begin(0x00);
 
}

unsigned t0 = 0;

void loop() {
  static uint8_t lastKey=0;
  int8_t key;

  encoders.tick();

  if (millis() - t0 > 100)  // display encoder values every 100 ms
  {
      for (unsigned i = 0; i < encoderCount; i++)
      {
          Serial.print("E");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(encoders[i].getValue());
      }
      Serial.println();
      t0 = millis();
  }

  delay(30);
  key=HT.readKey();
  if (key != 0) { // key is pressed
    if (key != lastKey){
      if(key<0) {
        Serial.print(F("Key released: "));
        Serial.println(-key);
      } else {
        Serial.print(F("Key pressed: "));
        Serial.println(key);
      }
      // Serial.print(F("Key pressed: "));
      // Serial.println(key);
      lastKey=key;
    }
  }
} // loop
