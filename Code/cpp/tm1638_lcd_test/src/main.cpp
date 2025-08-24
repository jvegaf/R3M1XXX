// TM1638 manual debug tool for segments and LEDs using Serial commands
// J1: 1=GND, 2=VCC, 3=SDA (not used), 4=SCL (not used), 5=DIO, 6=CLK, 7=STB

#include <Arduino.h>
#include "TM1638_Simple.hpp"

// Adjust these to your ESP32-C3 DevKitM-1 wiring
#ifndef PIN_TM_STB
#define PIN_TM_STB 4
#endif
#ifndef PIN_TM_CLK
#define PIN_TM_CLK 5
#endif
#ifndef PIN_TM_DIO
#define PIN_TM_DIO 6
#endif

TM1638Simple tm(PIN_TM_STB, PIN_TM_CLK, PIN_TM_DIO);

static const uint8_t NUM_POS = 6; // GR1..GR6 populated per PCB
static const char* SEG_NAMES[8] = {"A","B","C","D","E","F","G","DP"};

// Current UI state
static uint8_t curPos = 0;   // 0..5
static uint8_t curSeg = 0;   // 0..7 (A..DP)
static bool applyAll = false; // apply actions to all positions

// Display buffers
static uint8_t segBuf[NUM_POS] = {0};
static bool ledBuf[NUM_POS] = {false};

static void renderAll() {
	for (uint8_t p = 0; p < NUM_POS; ++p) {
		tm.setRawAt(p, segBuf[p], ledBuf[p]);
	}
}

static void printStatus() {
	Serial.print("pos="); Serial.print(curPos);
	Serial.print(" seg="); Serial.print(curSeg);
	Serial.print(" ("); Serial.print(SEG_NAMES[curSeg]); Serial.print(") mask=0x");
	Serial.print((uint8_t)(1u << curSeg), HEX);
	Serial.print(" all="); Serial.print(applyAll ? 1 : 0);
	Serial.print(" LEDs=");
	for (int8_t i = NUM_POS - 1; i >= 0; --i) Serial.print(ledBuf[i] ? '1' : '0');
	Serial.print(" segs=[");
	for (uint8_t i = 0; i < NUM_POS; ++i) {
		if (i) Serial.print(" ");
		Serial.print("0x"); Serial.print(segBuf[i], HEX);
	}
	Serial.println("]");
}

static void printHelp() {
	Serial.println("TM1638 manual debug controls:");
	Serial.println("  n / p    -> next / prev segment (A..G,DP) and apply");
	Serial.println("  > / <    -> next / prev digit (0..5)");
	Serial.println("  0..5     -> select digit directly");
	Serial.println("  a        -> toggle apply-to-all-digits");
	Serial.println("  l        -> toggle LED at current (or all if 'a' enabled)");
	Serial.println("  c        -> clear segments (keeps LEDs)");
	Serial.println("  h        -> help");
	Serial.println("Additionally, key scan is printed on change: Keys raw 0x???????? [b0 b1 b2 b3]");
}

static void applyActiveSegment() {
	uint8_t m = (uint8_t)(1u << curSeg);
	if (applyAll) {
		for (uint8_t p = 0; p < NUM_POS; ++p) segBuf[p] = m;
	} else {
		for (uint8_t p = 0; p < NUM_POS; ++p) segBuf[p] = (p == curPos) ? m : 0x00;
	}
	renderAll();
	printStatus();
}

void setup() {
	Serial.begin(115200);
	delay(200);
	Serial.println("TM1638 manual debug starting...");
	tm.begin(/*brightness*/4, /*displayOn*/true);

	// Initial render (all off)
	renderAll();
	printHelp();
	printStatus();
}

void loop() {
	// Handle serial commands
	while (Serial.available() > 0) {
		int ch = Serial.read();
		if (ch == -1) break;
		switch (ch) {
			case 'n': // next segment
				curSeg = (uint8_t)((curSeg + 1) & 0x07);
				applyActiveSegment();
				break;
			case 'p': // prev segment
				curSeg = (uint8_t)((curSeg + 7) & 0x07);
				applyActiveSegment();
				break;
			case '>': // next digit
				curPos = (uint8_t)((curPos + 1) % NUM_POS);
				applyActiveSegment();
				break;
			case '<': // prev digit
				curPos = (uint8_t)((curPos + NUM_POS - 1) % NUM_POS);
				applyActiveSegment();
				break;
			case '0': case '1': case '2': case '3': case '4': case '5': {
				uint8_t d = (uint8_t)(ch - '0');
				if (d < NUM_POS) {
					curPos = d;
					applyActiveSegment();
				}
				break; }
			case 'a': // toggle apply to all
				applyAll = !applyAll;
				applyActiveSegment();
				break;
			case 'l': { // toggle LED(s)
				bool newVal = !ledBuf[curPos];
				if (applyAll) {
					for (uint8_t p = 0; p < NUM_POS; ++p) ledBuf[p] = newVal;
				} else {
					ledBuf[curPos] = newVal;
				}
				renderAll();
				printStatus();
				break; }
			case 'c': // clear segments (keep LEDs)
				for (uint8_t p = 0; p < NUM_POS; ++p) segBuf[p] = 0x00;
				renderAll();
				printStatus();
				break;
			case 'h':
				printHelp();
				break;
			case '\n': case '\r':
				// ignore
				break;
			default:
				// echo unrecognized
				Serial.print("? "); Serial.write((char)ch); Serial.println();
				break;
		}
	}

	// Periodic key scan log
	static uint32_t lastKeys = 0;
	static uint32_t lastPoll = 0;
	uint32_t now = millis();
	if (now - lastPoll >= 100) {
		lastPoll = now;
		uint32_t k = tm.readKeysRaw();
		if (k != lastKeys) {
			lastKeys = k;
			Serial.print("Keys raw: 0x"); Serial.println(k, HEX);
			uint8_t b0 = (uint8_t)(k & 0xFF);
			uint8_t b1 = (uint8_t)((k >> 8) & 0xFF);
			uint8_t b2 = (uint8_t)((k >> 16) & 0xFF);
			uint8_t b3 = (uint8_t)((k >> 24) & 0xFF);
			Serial.print("[b0 b1 b2 b3] = [");
			Serial.print(b0, HEX); Serial.print(" ");
			Serial.print(b1, HEX); Serial.print(" ");
			Serial.print(b2, HEX); Serial.print(" ");
			Serial.print(b3, HEX); Serial.println("]");
		}
	}
}

