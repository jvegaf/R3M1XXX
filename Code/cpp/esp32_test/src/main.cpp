#include <Arduino.h>
#include "EncoderTool.h"
using namespace EncoderTool;

constexpr unsigned encoderCount = 8;  // number of attached encoders

constexpr unsigned S0 = 18;            //address pin 0
constexpr unsigned S1 = 19;            //...
constexpr unsigned S2 = 21;            //address pin 2
constexpr unsigned SIG_A = 12;         //output pin SIG of multiplexer A
constexpr unsigned SIG_B = 13;         //output pin SIG of multiplexer B
                                      //breakout:  https://www.sparkfun.com/products/13906
                                      //datasheet: https://www.ti.com/lit/gpn/CD74HC4051

EncPlex4051 encoders(encoderCount, S0, S1, S2, SIG_A, SIG_B);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    encoders.begin();
}

unsigned t0 = 0;

void loop()
{
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
}