#include <Arduino.h>
#include "pins.h"
#include "mux.h"

Mux mux_pad(MUX_SIG, (uint8_t[]){MUX_S0, MUX_S1, MUX_S2, MUX_S3});


void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
    }
    Serial.println("Hello, World!");
    mux_pad.begin();
    Serial.println("muxpad initializated");
}

void loop() {
    mux_pad.read([](uint8_t channel) {
        Serial.print("Channel ");
        Serial.print(channel);
        Serial.println(" activated");
    }, [](uint8_t channel) {
        Serial.print("Channel ");
        Serial.print(channel);
        Serial.println(" deactivated");
    });
    // put your main code here, to run repeatedly:
}