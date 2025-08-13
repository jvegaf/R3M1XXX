#include <Arduino.h>
#include <Wire.h>

// Dirección I2C del HT16K33 (solo uno controlando ambos displays)
#define HT16K33_ADDRESS 0x70

// Pines I2C para Raspberry Pi Pico (por defecto en arduino-mbed)
// SDA: GPIO 4 (Pin 6)
// SCL: GPIO 5 (Pin 7)

// Comandos para HT16K33
#define HT16K33_BLINK_CMD 0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_CMD_BRIGHTNESS 0xE0
#define HT16K33_OSCILLATOR 0x21

// Buffer para el HT16K33 (16 bytes) (movido arriba para que showNumber lo vea)
uint8_t displayBuffer[16] = {0};

// ================== Configuración flexible de segmentos ==================
// Convención lógica: bits 0..6 = a..g, bit 7 = punto decimal.
// Ajusta 'segmentMap' si el cableado físico del módulo no coincide con ese orden.
// segmentMap[i] = índice de bit físico (0..7) que controla el segmento lógico i.
// Mapeo confirmado: bits físicos 0..7 corresponden directamente a a,b,c,d,e,f,g,dp
// Ajusta si cambias el hardware.
static uint8_t segmentMap[8] = {2,3,4,5,6,7,0,1}; // a,b,c,d,e,f,g,dp
// Cambia a false si los segmentos son activos en bajo (0 enciende LED).
static bool segmentsActiveHigh = true;

// Patrones lógicos estándar para 0-F (sin mapping ni inversión todavía):
static const uint8_t digitTableLogical[] = {
  0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
  0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71
};

// Aplica mapping lógico->físico y lógica activa.
uint8_t encodeSegments(uint8_t logicalPattern) {
  uint8_t physical = 0;
  for (uint8_t i=0;i<8;i++) {
    if (logicalPattern & (1<<i)) {
      uint8_t phys = segmentMap[i];
      if (phys < 8) physical |= (1<<phys);
    }
  }
  if (!segmentsActiveHigh) {
    // Invertimos sólo a..g (sin DP). Usa 0xFF si también quieres invertir el punto decimal.
    physical = (~physical) & 0x7F;
  }
  return physical;
}

uint8_t encodeDigit(uint8_t d) {
  if (d < 16) return encodeSegments(digitTableLogical[d]);
  return 0;
}

// ===== Forward declarations for functions used before their definitions =====
void showTestPattern(uint8_t logicalPattern);
void clearDisplays();
void updateDisplay();

// ================== MODO / ESTADOS ==================
enum RunMode { MODE_DIAG = 0, MODE_DEMO = 1 };
RunMode currentMode = MODE_DIAG;

// ====== Demo (contador y patrones) ======
int demoCounter = 0;
unsigned long lastDemoUpdate = 0;
unsigned long demoInterval = 1000; // ms

void showNumber(uint8_t displayNum, int number) {
  if (displayNum > 1) return;
  uint8_t comOffset = displayNum * 3;
  if (number < 0 || number > 999) {
    uint8_t pattE = encodeDigit(0x0E); // E
    uint8_t pattR = encodeSegments((1<<3)|(1<<4)|(1<<6)); // d,e,g
    displayBuffer[comOffset * 2] = pattE;
    displayBuffer[(comOffset + 1) * 2] = pattR;
    displayBuffer[(comOffset + 2) * 2] = pattR;
    return;
  }
  uint8_t hundreds = (number / 100) % 10;
  uint8_t tens = (number / 10) % 10;
  uint8_t units = number % 10;
  displayBuffer[comOffset * 2] = (hundreds > 0 || number >= 100) ? encodeDigit(hundreds) : 0x00;
  if (tens > 0 || number >= 10) {
    displayBuffer[(comOffset + 1) * 2] = encodeDigit(tens);
  } else if (number >= 100) {
    displayBuffer[(comOffset + 1) * 2] = encodeDigit(0);
  } else {
    displayBuffer[(comOffset + 1) * 2] = 0x00;
  }
  displayBuffer[(comOffset + 2) * 2] = encodeDigit(units);
}

void runDemoStep() {
  unsigned long now = millis();
  if (now - lastDemoUpdate < demoInterval) return;
  lastDemoUpdate = now;

  // Secuencia simple: 0-99 iguales, 100-199 invertidos, 200-299 separados, 300-309 todos segmentos, 310-317 segmentos individuales
  if (demoCounter < 100) {
    showNumber(0, demoCounter);
    showNumber(1, demoCounter);
  } else if (demoCounter < 200) {
    int v = demoCounter - 100; // <- restaurada línea completa
    showNumber(0, v);
    showNumber(1, 99 - v);
  } else if (demoCounter < 300) {
    int v = demoCounter - 200;
    showNumber(0, v);
    showNumber(1, v + 100);
  } else if (demoCounter < 310) {
    showTestPattern(0x7F); // todos a-g
  } else if (demoCounter < 318) {
    int seg = demoCounter - 310;
    showTestPattern(1 << seg);
  } else {
    demoCounter = -1;
    clearDisplays();
  }
  updateDisplay();
  demoCounter++;
}

// ===== Variables para diagnóstico de asignación de segmentos =====
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 1000; // ms entre pasos
uint8_t currentSegmentIndex = 0; // 0..6 (o 7 si se incluye DP)
const bool includeDecimalPointInTest = false; // pon a true si quieres probar el punto decimal
const char* segmentNames[8] = {"a","b","c","d","e","f","g","dp"};
bool autoAdvance = true;
int8_t observedMap[8] = {-1,-1,-1,-1,-1,-1,-1,-1};

void printHelp();
void printMappingSummary();
void showCurrentSegment();

// Función para escribir comando al HT16K33
void writeCommand(uint8_t address, uint8_t cmd) {
  Wire.beginTransmission(address);
  Wire.write(cmd);
  Wire.endTransmission();
}

// Función para inicializar el display HT16K33
bool initDisplay() {
  // Activar oscilador
  writeCommand(HT16K33_ADDRESS, HT16K33_OSCILLATOR);
  delay(10);
  
  // Activar display
  writeCommand(HT16K33_ADDRESS, HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON);
  delay(10);
  
  // Configurar brillo (0-15)
  writeCommand(HT16K33_ADDRESS, HT16K33_CMD_BRIGHTNESS | 8);
  delay(10);
  
  return true;
}

// Función para escribir datos al display
void updateDisplay() {
  Wire.beginTransmission(HT16K33_ADDRESS);
  Wire.write(0x00); // Comenzar desde la dirección 0
  for (uint8_t i = 0; i < 16; i++) {
    Wire.write(displayBuffer[i]);
  }
  Wire.endTransmission();
}

// Función para limpiar ambos displays
void clearDisplays() {
  for (int i = 0; i < 16; i++) {
    displayBuffer[i] = 0;
  }
  updateDisplay();
}

// Función para mostrar un patrón de test en ambos displays
void showTestPattern(uint8_t logicalPattern) {
  uint8_t mapped = encodeSegments(logicalPattern);
  for (int display = 0; display < 2; display++) {
    uint8_t comOffset = display * 3;
    for (int digit = 0; digit < 3; digit++) {
      displayBuffer[(comOffset + digit) * 2] = mapped;
    }
  }
  updateDisplay();
}

void showCurrentSegment() {
  uint8_t maxSeg = includeDecimalPointInTest ? 8 : 7;
  uint8_t logicalSegment = currentSegmentIndex % maxSeg;
  showTestPattern(1 << logicalSegment);
  Serial.print("[SEG] Lógico ");
  Serial.print(logicalSegment);
  Serial.print(" (" ); Serial.print(segmentNames[logicalSegment]); Serial.print(") -> físico esperado? ");
  if (observedMap[logicalSegment] >= 0) {
    Serial.print(observedMap[logicalSegment]);
  } else {
    Serial.print("(sin registrar)");
  }
  Serial.print(" | ActiveHigh="); Serial.print(segmentsActiveHigh ? "sí" : "no");
  Serial.print(" | Avance="); Serial.println(autoAdvance ? "auto" : "manual");
}

// Muestra directamente un bit físico (0..7) sin pasar por el mapeo lógico
void showPhysicalBit(uint8_t physBit) {
  uint8_t mask = (physBit < 8) ? (1 << physBit) : 0;
  for (int display = 0; display < 2; display++) {
    uint8_t comOffset = display * 3;
    for (int digit = 0; digit < 3; digit++) {
      displayBuffer[(comOffset + digit) * 2] = mask;
    }
  }
  updateDisplay();
  Serial.print("[RAW] Bit físico "); Serial.print(physBit); Serial.println(" activado en todos los dígitos.");
}

void printMappingSummary() {
  Serial.println("\n=== Resumen de mapeo observado ===");
  uint8_t maxSeg = includeDecimalPointInTest ? 8 : 7;
  for (uint8_t i=0;i<maxSeg;i++) {
    Serial.print(segmentNames[i]); Serial.print(" -> ");
    if (observedMap[i] >= 0) Serial.println(observedMap[i]); else Serial.println("(sin asignar)");
  }
  Serial.println("Propuesta de segmentMap (sustituir en código):");
  Serial.print("static uint8_t segmentMap[8] = { ");
  for (uint8_t i=0;i<8;i++) {
    if (i) Serial.print(", ");
    if (observedMap[i] >= 0) Serial.print(observedMap[i]); else Serial.print("/*?*/0");
  }
  Serial.println(" };\n");
}

void printHelp() {
  Serial.println("Comandos disponibles:");
  Serial.println("  h/help         : mostrar ayuda");
  Serial.println("  n/next         : siguiente segmento (manual)");
  Serial.println("  p/prev         : segmento anterior (manual)");
  Serial.println("  a/auto         : alternar avance automático/manual");
  Serial.println("  inv/invert     : alternar activeHigh (invertir lógica)");
  Serial.println("  phys X         : registrar bit físico para segmento lógico actual (0-7)");
  Serial.println("  map            : mostrar resumen y plantilla segmentMap");
  Serial.println("  resetmap       : limpiar mapeo observado");
  Serial.println("  show           : re-mostrar segmento actual");
  Serial.println("  mode diag      : cambiar a modo diagnóstico");
  Serial.println("  mode demo      : cambiar a modo demo (contador y patrones)");
  Serial.println("  num A B        : (modo demo) fija números A y B (0-999) inmediatamente");
  Serial.println("  speed MS       : (modo demo) cambia intervalo ms (>=50)");
  Serial.println("  resetdemo      : reinicia secuencia demo");
  Serial.println();
}

// Función para escanear dispositivos I2C
void scanI2C() {
  Serial.println("Escaneando dispositivos I2C...");
  byte error, address;
  int nDevices = 0;
  
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("Dispositivo I2C encontrado en dirección 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    }
  }
  
  if (nDevices == 0) {
    Serial.println("No se encontraron dispositivos I2C");
  } else {
    Serial.print("Encontrados ");
    Serial.print(nDevices);
    Serial.println(" dispositivos");
  }
  Serial.println();
}

void setup() {
  // Inicializar comunicación serie
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("=== HT16K33 Utilidad (modo diagnóstico inicial) ===");
  Serial.println("Modo diag: rota segmentos lógicos para calibrar mapping.");
  Serial.println("Cambia a modo demo con: mode demo");
  Serial.println();
  
  // Inicializar I2C (usa pines por defecto: SDA=GP4, SCL=GP5)
  Wire.begin();
  delay(100);
  
  // Escanear dispositivos I2C
  scanI2C();
  
  // Inicializar el controlador HT16K33
  Serial.println("Inicializando controlador HT16K33...");
  
  bool display_ok = initDisplay();
  
  if (display_ok) {
    Serial.println("Controlador HT16K33 inicializado correctamente (0x70)");
  } else {
    Serial.println("Error: No se pudo inicializar el controlador HT16K33");
  }
  
  // Mostrar test inicial - todos los segmentos encendidos
  Serial.println("Escribe 'help' para ver comandos. Paso inicial: segmento 'a'.");
  showCurrentSegment();
  delay(1000);
  
  // Limpiar displays
  clearDisplays();
  delay(500);
  
  Serial.println("En modo diag auto cada segundo cambia el segmento. Orden lógico: a b c d e f g" );
  if (includeDecimalPointInTest) Serial.println("(Incluyendo punto decimal al final)");
  Serial.println("Si la asignación no coincide, edita segmentMap y vuelve a cargar.");
  printHelp();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Procesar entrada serie si hay
  static String cmdLine;
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      cmdLine.trim();
      if (cmdLine.length()) {
        String lower = cmdLine; lower.toLowerCase();
        if (lower == "h" || lower == "help") {
          printHelp();
        } else if (lower == "n" || lower == "next") {
          autoAdvance = false; currentSegmentIndex++; showCurrentSegment();
        } else if (lower == "p" || lower == "prev") {
          autoAdvance = false; currentSegmentIndex = (currentSegmentIndex + (includeDecimalPointInTest?8:7) - 1) % (includeDecimalPointInTest?8:7); showCurrentSegment();
        } else if (lower == "a" || lower == "auto") {
          autoAdvance = !autoAdvance; Serial.print("[INFO] Modo auto = "); Serial.println(autoAdvance?"ON":"OFF");
        } else if (lower == "inv" || lower == "invert") {
          segmentsActiveHigh = !segmentsActiveHigh; Serial.print("[INFO] activeHigh ahora = "); Serial.println(segmentsActiveHigh?"true":"false"); showCurrentSegment();
        } else if (lower.startsWith("phys")) {
          int space = lower.indexOf(' ');
          if (space > 0) {
            int val = lower.substring(space+1).toInt();
            if (val >=0 && val < 8) {
              uint8_t maxSeg = includeDecimalPointInTest ? 8 : 7;
              uint8_t logicalSegment = currentSegmentIndex % maxSeg;
              observedMap[logicalSegment] = val;
              segmentMap[logicalSegment] = val;
              Serial.print("[OK] Registrado segmento lógico "); Serial.print(segmentNames[logicalSegment]); Serial.print(" -> físico "); Serial.println(val);
              printMappingSummary();
            } else {
              Serial.println("[ERR] Valor físico fuera de rango (0-7)");
            }
          } else {
            Serial.println("Uso: phys <bit_fisico>");
          }
        } else if (lower == "map") {
          printMappingSummary();
        } else if (lower.startsWith("raw")) {
          int space = lower.indexOf(' ');
          if (space > 0) {
            int val = lower.substring(space+1).toInt();
            if (val>=0 && val<8) {
              showPhysicalBit(val);
            } else {
              Serial.println("[ERR] raw <0-7>");
            }
          } else {
            Serial.println("Uso: raw <bit_fisico_0-7>");
          }
        } else if (lower.startsWith("setmap")) {
          int firstSpace = lower.indexOf(' ');
            if (firstSpace > 0) {
              String rest = lower.substring(firstSpace+1);
              rest.trim();
              int secondSpace = rest.indexOf(' ');
              if (secondSpace > 0) {
                int L = rest.substring(0, secondSpace).toInt();
                int P = rest.substring(secondSpace+1).toInt();
                if (L>=0 && L<8 && P>=0 && P<8) {
                  segmentMap[L] = (uint8_t)P;
                  Serial.print("[OK] segmentMap["); Serial.print(L); Serial.print("] = "); Serial.println(P);
                  showCurrentSegment();
                } else {
                  Serial.println("[ERR] Rango: L 0-7, P 0-7");
                }
              } else {
                Serial.println("Uso: setmap <logico 0-7> <fisico 0-7>");
              }
            } else {
              Serial.println("Uso: setmap <logico> <fisico>");
            }
        } else if (lower == "resetmap") {
          for (int i=0;i<8;i++) observedMap[i] = -1; Serial.println("[INFO] Mapeo observado reiniciado.");
        } else if (lower == "show") {
          showCurrentSegment();
        } else if (lower.startsWith("mode")) {
          int space = lower.indexOf(' ');
          if (space > 0) {
            String m = lower.substring(space+1);
            if (m == "diag") { currentMode = MODE_DIAG; Serial.println("[MODE] Diagnóstico"); showCurrentSegment(); }
            else if (m == "demo") { currentMode = MODE_DEMO; Serial.println("[MODE] Demo"); clearDisplays(); demoCounter = 0; lastDemoUpdate = 0; }
            else Serial.println("[ERR] mode diag|demo");
          } else Serial.println("Uso: mode <diag|demo>");
        } else if (lower.startsWith("num")) {
          if (currentMode != MODE_DEMO) { Serial.println("[ERR] num sólo en modo demo"); }
          else {
            int firstSpace = lower.indexOf(' ');
            if (firstSpace > 0) {
              String rest = lower.substring(firstSpace+1); rest.trim();
              int space2 = rest.indexOf(' ');
              if (space2 > 0) {
                int aVal = rest.substring(0, space2).toInt();
                int bVal = rest.substring(space2+1).toInt();
                showNumber(0, aVal); showNumber(1, bVal); updateDisplay();
                Serial.print("[NUM] D0="); Serial.print(aVal); Serial.print(" D1="); Serial.println(bVal);
              } else Serial.println("Uso: num <A 0-999> <B 0-999>");
            } else Serial.println("Uso: num <A> <B>");
          }
        } else if (lower.startsWith("speed")) {
          if (currentMode != MODE_DEMO) { Serial.println("[ERR] speed sólo en modo demo"); }
          else {
            int space = lower.indexOf(' ');
            if (space > 0) {
              unsigned long ms = lower.substring(space+1).toInt();
              if (ms >= 50) { demoInterval = ms; Serial.print("[SPEED] Intervalo demo = "); Serial.println(demoInterval); }
              else Serial.println("[ERR] speed mínimo 50 ms");
            } else Serial.println("Uso: speed <ms>");
          }
        } else if (lower == "resetdemo") {
          if (currentMode == MODE_DEMO) { demoCounter = 0; lastDemoUpdate = 0; Serial.println("[INFO] Demo reiniciada"); }
          else Serial.println("[ERR] resetdemo sólo en modo demo");
        } else {
          Serial.println("[WARN] Comando no reconocido. Escribe 'help'.");
        }
      }
      cmdLine = ""; // limpiar
    } else {
      cmdLine += c;
    }
  }

  if (currentMode == MODE_DIAG) {
    // Avance automático diagnóstico
    if (autoAdvance && (currentTime - lastUpdate >= UPDATE_INTERVAL)) {
      lastUpdate = currentTime;
      uint8_t maxSeg = includeDecimalPointInTest ? 8 : 7;
      currentSegmentIndex = (currentSegmentIndex + 1) % maxSeg;
      showCurrentSegment();
    }
  } else if (currentMode == MODE_DEMO) {
    runDemoStep();
  }
  
  // Pequeña pausa para no saturar el loop
  delay(10);
}