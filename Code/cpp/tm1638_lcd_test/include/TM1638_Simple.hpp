// Minimal, header-only TM1638 bit-banged driver for Arduino/ESP32
// Based on TM1638 protocol: commands 0x40/0x44, address 0xC0, display ctrl 0x88..0x8F
#pragma once
#include <Arduino.h>

class TM1638Simple {
public:
    TM1638Simple(uint8_t pinSTB, uint8_t pinCLK, uint8_t pinDIO)
        : stb(pinSTB), clk(pinCLK), dio(pinDIO) {}

    void begin(uint8_t brightness = 2, bool displayOn = true) {
        pinMode(stb, OUTPUT);
        pinMode(clk, OUTPUT);
        pinMode(dio, OUTPUT);
        digitalWrite(stb, HIGH);
        digitalWrite(clk, HIGH);
        digitalWrite(dio, HIGH);
        setDisplay(displayOn, brightness);
        clear();
    }

    void setDisplay(bool on, uint8_t brightness) {
        if (brightness > 7) brightness = 7;
        sendCommand(0x80 | (on ? 0x08 : 0x00) | brightness);
    }

    void clear() {
    uint8_t zeros[16] = {0};
    setRaw16(zeros);
    }

    void setRawAt(uint8_t pos /*0..7*/, uint8_t segMask, bool led = false) {
        // In TM1638 each position has two bytes: [pos*2]=segments, [pos*2+1]=LED
        sendCommand(0x44); // fixed address mode
        start();
        writeByte(0xC0 | (pos * 2));
        writeByte(segMask);
        stop();
        start();
        writeByte(0xC0 | (pos * 2 + 1));
        writeByte(led ? 0x01 : 0x00);
        stop();
    }

    void setRaw16(const uint8_t bytes16[16]) {
        sendCommand(0x40); // auto-inc
        start();
        writeByte(0xC0);
        for (int i = 0; i < 16; ++i) writeByte(bytes16[i]);
        stop();
    }

    // Show up to 8 digits (0..7 positions). Our PCB uses GR1..GR6, so 0..5.
    void showDigits(const int8_t digits[8], bool dots[8] = nullptr) {
        for (uint8_t i = 0; i < 8; ++i) {
            uint8_t m = encodeDigit(digits[i]);
            if (dots && dots[i]) m |= 0x80; // DP is bit7
            setRawAt(i, m, false);
        }
    }

    // Read 4 bytes key state (Kx vs GRID). Returns 32 bits raw.
    uint32_t readKeysRaw() {
        uint32_t val = 0;
        start();
        writeByte(0x42); // read key scan data
    pinMode(dio, INPUT_PULLUP);
        for (int i = 0; i < 4; ++i) {
            uint8_t b = readByte();
            val |= ((uint32_t)b) << (i * 8);
        }
        pinMode(dio, OUTPUT);
        stop();
        return val;
    }

    // Segment encoding assuming SEG1=A .. SEG7=G, SEG8=DP
    static uint8_t encodeDigit(int8_t d) {
        switch (d) {
            case 0: return 0x3F;
            case 1: return 0x06;
            case 2: return 0x5B;
            case 3: return 0x4F;
            case 4: return 0x66;
            case 5: return 0x6D;
            case 6: return 0x7D;
            case 7: return 0x07;
            case 8: return 0x7F;
            case 9: return 0x6F;
            case 10: return 0x77; // A
            case 11: return 0x7C; // b
            case 12: return 0x39; // C
            case 13: return 0x5E; // d
            case 14: return 0x79; // E
            case 15: return 0x71; // F
            default: return 0x00; // blank
        }
    }

private:
    uint8_t stb, clk, dio;

    void start() {
        digitalWrite(stb, LOW);
        delayMicroseconds(1);
    }
    void stop() {
        digitalWrite(stb, HIGH);
        delayMicroseconds(1);
    }
    void writeByte(uint8_t v) {
        for (int i = 0; i < 8; ++i) {
            digitalWrite(clk, LOW);
            digitalWrite(dio, (v & 0x01) ? HIGH : LOW);
            delayMicroseconds(2);
            digitalWrite(clk, HIGH);
            delayMicroseconds(2);
            v >>= 1; // LSB first
        }
        // ACK bit from TM1638: it pulls DIO low; we must release DIO and read while CLK high
        digitalWrite(clk, LOW);
    // Release DIO and enable pull-up so the line idles high while TM1638 drives it low for ACK
    pinMode(dio, INPUT_PULLUP);
        delayMicroseconds(2);
        digitalWrite(clk, HIGH);
        // optional read ACK here if needed
        delayMicroseconds(2);
        pinMode(dio, OUTPUT);
        digitalWrite(dio, HIGH); // idle high
    }
    uint8_t readByte() {
        uint8_t v = 0;
        for (int i = 0; i < 8; ++i) {
            // LSB first: latch by driving CLK high, sample while high
            digitalWrite(clk, LOW);
            delayMicroseconds(2);
            digitalWrite(clk, HIGH);
            delayMicroseconds(2);
            v |= (digitalRead(dio) ? 1 : 0) << i;
        }
        return v;
    }
    void sendCommand(uint8_t cmd) {
        start();
        writeByte(cmd);
        stop();
    }
};
