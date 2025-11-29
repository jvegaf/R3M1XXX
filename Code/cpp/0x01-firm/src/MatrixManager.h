#pragma once
#include <ht16k33.h>
#include <MIDI.h>
#include "config.h"

class MatrixManager {
private:
    HT16K33 ht;
    uint8_t address;
    uint8_t noteStart;
    uint16_t lastKeys; // Máscara de bits anterior (16 bits para 16 botones)

public:
    MatrixManager(uint8_t addr, uint8_t startNote) 
        : address(addr), noteStart(startNote), lastKeys(0) {}

    void begin() {
        ht.begin(address);
        ht.displayOn();
        ht.setBrightness(8); // Max 15
        // Importante: Configurar pines como entrada en el chip HT16K33
        // La librería lo suele manejar, pero a veces requiere llamar a config
    }

    // Lee botones y envía MIDI
    void updateInput(midi::MidiInterface<midi::SerialMIDI<Adafruit_USBD_MIDI>> &midi) {
        // readKeyMask devuelve un uint16_t donde cada bit es un botón
        // Verifica si tu versión de la librería tiene esta función exacta o readKeys()
        // La librería lpaseen suele usar readKeys() que devuelve bool, pero es lento llamar 16 veces.
        // Usaremos el método más eficiente disponible.
        
        // *Nota de Arquitecto*: La librería lpaseen guarda el estado en keys[]. 
        // Llamamos a readKeys() una vez para actualizar el buffer interno.
        uint16_t keyData[3];
        ht.readKeyRaw(keyData); 
        
        // Combine first 16 bits (Row 0 has 13 bits, Row 1 has 13 bits)
        // Assuming buttons are mapped to first 16 logical positions.
        // Let's just use the first word (13 bits) and part of second?
        // Or maybe the user hardware maps them differently.
        // Given the previous code: "for(int i=0; i<16; i++)"
        // I'll assume we want to check the first 16 available key slots.
        
        uint16_t currentKeys = keyData[0]; // First 13 keys
        // We need 3 more keys from keyData[1]
        currentKeys |= (keyData[1] & 0x07) << 13; 

        if (currentKeys != lastKeys) {
            for (int i = 0; i < 16; i++) {
                bool isPressed = (currentKeys >> i) & 1;
                bool wasPressed = (lastKeys >> i) & 1;

                if (isPressed && !wasPressed) {
                    midi.sendNoteOn(noteStart + i, 127, MIDI_CH);
                } else if (!isPressed && wasPressed) {
                    midi.sendNoteOff(noteStart + i, 0, MIDI_CH);
                }
            }
            lastKeys = currentKeys;
        }
    }

    // Enciende un LED de la matriz (para feedback)
    void setLed(uint8_t ledIndex, bool state) {
        if (state) {
            ht.setLed(ledIndex);
        } else {
            ht.clearLed(ledIndex);
        }
        ht.sendLed(); // Enviar cambio al chip
    }
};