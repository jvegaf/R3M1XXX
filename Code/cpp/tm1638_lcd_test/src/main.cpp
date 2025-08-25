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
static bool displayOn = true;
static uint8_t displayBrightness = 4;

// Remapeo de dígitos UI -> índice crudo (GRID) del TM1638
// En tu placa parece que GR1 no está poblado; usamos GR2..GR7
static uint8_t DIG_MAP[NUM_POS] = { 0, 1, 2, 3, 4, 5 };

// Remapeo de bits de segmentos (UI -> crudo TM1638)
// Observado: los segmentos estaban desplazados una posición (A->B, B->C, ...)
// Este mapa aplica un desplazamiento +1 (DP pasa a A)
// Índice: UI (0=A..7=DP) -> Valor: bit crudo (0..7)
// Mapa correcto según tu observación: al escribir bit0 se enciende B, bit7 enciende A.
// Por tanto, UI->RAW: A->7, B->0, C->1, D->2, E->3, F->4, G->5, DP->6
static uint8_t SEG_MAP[8] = { 7, 0, 1, 2, 3, 4, 5, 6 };

static inline uint8_t mapUiMask(uint8_t uiMask) {
	uint8_t raw = 0;
	for (uint8_t i = 0; i < 8; ++i) {
		if (uiMask & (1u << i)) raw |= (1u << SEG_MAP[i]);
	}
	return raw;
}

static int readCharBlocking(uint32_t timeoutMs = 0) {
	uint32_t t0 = millis();
	while (true) {
		if (Serial.available() > 0) return Serial.read();
		if (timeoutMs && (millis() - t0) > timeoutMs) return -1;
		delay(5);
	}
}

static void renderAll() {
	uint8_t frame[16] = {0};
	for (uint8_t p = 0; p < NUM_POS; ++p) {
		uint8_t r = DIG_MAP[p];
		frame[r * 2] = mapUiMask(segBuf[p]);
		frame[r * 2 + 1] = ledBuf[p] ? 0x01 : 0x00;
	}
	tm.setRaw16(frame);
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
	Serial.println("  C        -> clear segments + LEDs (full)");
	Serial.println("  x        -> toggle display on/off");
	Serial.println("  T        -> raw segment sweep (diagnostic)");
	Serial.println("  K        -> calibrate segment map (UI A..G,DP)");
	Serial.println("  G        -> calibrate digit map (UI pos 0..5)");
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
	Serial.begin(9600);
	// Para ESP32-C3 con USB CDC, damos más tiempo a la enumeración
	uint32_t t0 = millis();
	while (!Serial && (millis() - t0) < 2000) {
		delay(10);
	}

	// Repetimos el banner unos segundos para captar monitor tardío
	for (int i = 0; i < 5; ++i) {
			Serial.println("=== TM1638 (modo diagnóstico) ===");
			Serial.println("Consejo: pulsa 'h' en el monitor serie para ver la ayuda.");
			delay(200);
		}
	tm.begin(/*brightness*/4, /*displayOn*/true);
	// En algunos TM1638 quedan residuos al encender; fuerza un frame limpio
	for (uint8_t i = 0; i < NUM_POS; ++i) { segBuf[i] = 0; ledBuf[i] = false; }
	renderAll();

	// Arranque en blanco: espera comandos del usuario
	printHelp();
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
			case 'C': // full clear segments + LEDs (diagnóstico)
				for (uint8_t p = 0; p < NUM_POS; ++p) { segBuf[p] = 0x00; ledBuf[p] = false; }
				renderAll();
				printStatus();
				break;
			case 'x': // toggle display on/off (diagnóstico)
				displayOn = !displayOn;
				tm.setDisplay(displayOn, displayBrightness);
				Serial.print("Display "); Serial.println(displayOn ? "ON" : "OFF");
				break;
			case 'h':
				printHelp();
				break;
			case 'T': { // Raw test: barre 0..7 bits crudos sin mapeo en el dígito actual
				Serial.println("Test crudo de segmentos (0..7) en el dígito actual");
				uint8_t savedSeg = segBuf[curPos];
				bool savedLed = ledBuf[curPos];
				uint8_t rawPos = DIG_MAP[curPos];
				for (uint8_t r = 0; r < 8; ++r) {
					Serial.print("RAW bit "); Serial.println(r);
					tm.setRawAt(rawPos, (uint8_t)(1u << r), false);
					delay(400);
				}
				// Restaurar estado
				renderAll();
				break; }
			case 'K': { // Calibración de segmentos UI->RAW en el dígito actual
				Serial.println("Calibración de segmentos. Para cada RAW bit, introduce la letra que ves: A,B,C,D,E,F,G,P (P=DP)");
				uint8_t rawPos = DIG_MAP[curPos];
				int8_t uiForRaw[8]; for (int i=0;i<8;++i) uiForRaw[i] = -1;
				for (uint8_t r = 0; r < 8; ++r) {
					// Mostrar solo ese bit
					tm.setRawAt(rawPos, (uint8_t)(1u << r), false);
					Serial.print("RAW "); Serial.print(r); Serial.print(" -> ");
					int c;
					while (true) {
						c = readCharBlocking();
						if (c >= 'a' && c <= 'z') c -= 32;
						int8_t ui = -1;
						switch (c) {
							case 'A': ui = 0; break; case 'B': ui = 1; break; case 'C': ui = 2; break; case 'D': ui = 3; break;
							case 'E': ui = 4; break; case 'F': ui = 5; break; case 'G': ui = 6; break; case 'P': ui = 7; break;
							default: break;
						}
						if (ui != -1) { uiForRaw[r] = ui; break; }
					}
				}
				// Construir SEG_MAP (UI->RAW)
				for (uint8_t ui = 0; ui < 8; ++ui) {
					int8_t raw = -1;
					for (uint8_t r = 0; r < 8; ++r) if (uiForRaw[r] == (int8_t)ui) { raw = r; break; }
					if (raw >= 0) SEG_MAP[ui] = (uint8_t)raw;
				}
				Serial.print("SEG_MAP actualizado: {");
				for (int i=0;i<8;++i){ if(i) Serial.print(", "); Serial.print(SEG_MAP[i]); }
				Serial.println("}");
				// Restaurar
				renderAll();
				break; }
			case 'G': { // Calibración de dígitos UI->RAW GRID
				Serial.println("Calibración de dígitos. Para cada GRID crudo 0..7, introduce la posición UI (0..5) o X si no usado");
				int8_t uiForRaw[8]; for (int i=0;i<8;++i) uiForRaw[i] = -1;
				for (uint8_t r = 0; r < 8; ++r) {
					// Enciende un segmento cualquiera (bit 0) en ese GRID
					tm.setRawAt(r, 0x01, false);
					Serial.print("GRID RAW "); Serial.print(r); Serial.print(" -> UI pos (0..5/X): ");
					int c;
					while (true) {
						c = readCharBlocking();
						if (c == 'X' || c == 'x') { uiForRaw[r] = -1; break; }
						if (c >= '0' && c <= '5') { uiForRaw[r] = (int8_t)(c - '0'); break; }
					}
					// apaga ese GRID
					tm.setRawAt(r, 0x00, false);
				}
				// Construir DIG_MAP (UI->RAW)
				for (uint8_t ui = 0; ui < NUM_POS; ++ui) {
					int8_t raw = -1;
					for (uint8_t r = 0; r < 8; ++r) if (uiForRaw[r] == (int8_t)ui) { raw = r; break; }
					if (raw >= 0) DIG_MAP[ui] = (uint8_t)raw;
				}
				Serial.print("DIG_MAP actualizado: {");
				for (int i=0;i<NUM_POS;++i){ if(i) Serial.print(", "); Serial.print(DIG_MAP[i]); }
				Serial.println("}");
				// Restaurar
				renderAll();
				break; }
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

	// Sin heartbeat para evitar confusiones durante el diagnóstico
}

