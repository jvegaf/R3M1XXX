#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#include "config.h"
#include "PotManager.h"
#include "MatrixManager.h"
#include "EncoderManager.h"

// --- USB MIDI SETUP ---
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

// --- PERIFÉRICOS ---
PotManager pots;
MatrixManager mainMatrix(ADDR_MATRIX_MAIN, NOTE_MAIN_START);
MatrixManager auxMatrix(ADDR_MATRIX_AUX, NOTE_AUX_START);

// Encoders
EncoderManager enc1(ADDR_ENC_1, CC_ENC_START, NOTE_ENC_START);
EncoderManager enc2(ADDR_ENC_2, CC_ENC_START + 1, NOTE_ENC_START + 1);
EncoderManager enc3(ADDR_ENC_3, CC_ENC_START + 2, NOTE_ENC_START + 2);

// Para el display de 7 segmentos, usaremos la instancia cruda de la lib
HT16K33 segDisplay;

// NeoPixels
Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// --- VARIABLES COMPARTIDAS (Inter-Core) ---
// Usamos 'volatile' para indicar que cambian entre núcleos
volatile uint8_t current_animation = 0; 

// -----------------------------------------------------------------
// CORE 1: RESPONSABLE DE LUCES (WS2812)
// -----------------------------------------------------------------
void setup1() {
    pixels.begin();
    pixels.setBrightness(LED_BRIGHTNESS);
    pixels.show();
}

void loop1() {
    // Animación Arcoiris suave (No bloqueante para el Core 0)
    static uint16_t hue = 0;
    
    for(int i=0; i<pixels.numPixels(); i++) {
        // Un arcoiris rotatorio
        int pixelHue = hue + (i * 65536L / pixels.numPixels());
        pixels.setPixelColor(i, pixels.ColorHSV(pixelHue));
    }
    pixels.show();
    
    hue += 256; // Velocidad de rotación
    delay(20);  // ~50 FPS. Aquí el delay NO afecta al MIDI.
}

// -----------------------------------------------------------------
// CORE 0: RESPONSABLE DE MIDI E I2C (Lógica de Negocio)
// -----------------------------------------------------------------
void setup() {
    // 1. Configurar I2C
    Wire.setSDA(PIN_I2C_SDA);
    Wire.setSCL(PIN_I2C_SCL);
    Wire.begin();
    
    // NOTA: 1MHz es posible, pero 400kHz es el estándar seguro para HT16K33.
    // Si tienes cables cortos (<10cm), prueba 1000000. Si hay fallos, baja a 400000.
    Wire.setClock(1000000); 

    // 2. Configurar USB MIDI
    USBDevice.setProductDescriptor("Pico DJ Controller");
    USBDevice.begin();
    MIDI.begin(MIDI_CHANNEL_OMNI);

    // 3. Iniciar Chips
    if (!pots.begin()) {
        // Manejo de error (ej. parpadear led onboard)
    }
    
    mainMatrix.begin();
    auxMatrix.begin();
    
    enc1.begin();
    enc2.begin();
    enc3.begin();
    
    segDisplay.begin(ADDR_DISPLAY);
    segDisplay.displayOn();
    segDisplay.setBrightness(10);
}

void loop() {
    // Leer MIDI entrante (para feedback desde el DAW)
    if (MIDI.read()) {
        // Ejemplo: Si llega NoteOn para el botón 1, encender su LED en la matriz
        // int note = MIDI.getData1();
        // mainMatrix.setLed(note - NOTE_MAIN_START, true);
    }

    // Procesar Inputs (Esto corre a toda velocidad posible)
    pots.update(MIDI);
    mainMatrix.updateInput(MIDI);
    auxMatrix.updateInput(MIDI);
    
    enc1.update(MIDI);
    enc2.update(MIDI);
    enc3.update(MIDI);

    // Actualizar Display 7 segmentos (ejemplo: mostrar tiempo en ms o un valor fijo)
    // Hacer esto cada ciclo es demasiado rápido para el ojo y el bus I2C.
    // Hagámoslo cada 100ms.
    static unsigned long lastDisplayUpdate = 0;
    if (millis() - lastDisplayUpdate > 100) {
        // segDisplay.displayInt(128); // Ejemplo: Mostrar BPM fijos
        // Usamos set7Seg(digit, value, dp)
        segDisplay.set7Seg(0, 1, false);
        segDisplay.set7Seg(1, 2, false);
        segDisplay.set7Seg(3, 8, false); // Digit 2 is usually colon
        segDisplay.sendLed();
        lastDisplayUpdate = millis();
    }

    // NO usar delay() aquí. El loop debe ser infinito y rápido.
}