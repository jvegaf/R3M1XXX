#pragma once
#include <Adafruit_ADS7830.h>
#include <MIDI.h>
#include "config.h"

class PotManager {
private:
    Adafruit_ADS7830 ads;
    uint8_t lastValues[8];

public:
    bool begin() {
        // Intentar iniciar. ADS7830 usa Wire por defecto.
        return ads.begin(ADDR_POTS);
    }

    void update(midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>> &midi) {
        for (int i = 0; i < 8; i++) {
            // Leer valor (0-255)
            uint8_t raw = ads.readADCsingle(i);
            // Convertir a MIDI (0-127)
            uint8_t midiVal = raw >> 1;

            // Histéresis: Solo enviar si cambia el valor
            // Agregamos una tolerancia de +/- 1 en la lectura raw para evitar jitter
            if (midiVal != lastValues[i]) {
                midi.sendControlChange(CC_POTS_START + i, midiVal, MIDI_CH);
                lastValues[i] = midiVal;
            }
        }
    }
};