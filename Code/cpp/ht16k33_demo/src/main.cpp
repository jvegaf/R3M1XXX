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

// Tabla de dígitos para display de 7 segmentos (cátodo común)
const uint8_t digitTable[] = {
  0x3F, // 0
  0x06, // 1
  0x5B, // 2
  0x4F, // 3
  0x66, // 4
  0x6D, // 5
  0x7D, // 6
  0x07, // 7
  0x7F, // 8
  0x6F, // 9
  0x77, // A
  0x7C, // b
  0x39, // C
  0x5E, // d
  0x79, // E
  0x71  // F
};

// Variables para el contador y estado
int counter = 0;
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 1000; // Actualizar cada segundo

// Buffer para el HT16K33 (16 bytes)
// El HT16K33 maneja una matriz de 16x8, cada byte representa una fila
// Para 2 displays de 3 dígitos: 6 dígitos totales
// Display 1: dígitos en COM0, COM1, COM2
// Display 2: dígitos en COM3, COM4, COM5
uint8_t displayBuffer[16] = {0};

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

// Función para mostrar un número de 3 dígitos en el display especificado
// displayNum: 0 = primer display (COM0-COM2), 1 = segundo display (COM3-COM5)
void showNumber(uint8_t displayNum, int number) {
  if (displayNum > 1) return; // Solo tenemos 2 displays
  
  uint8_t comOffset = displayNum * 3; // COM0-2 para display 0, COM3-5 para display 1
  
  if (number < 0 || number > 999) {
    // Mostrar error si el número está fuera de rango
    displayBuffer[comOffset * 2] = 0x79;     // E en primer dígito
    displayBuffer[(comOffset + 1) * 2] = 0x50; // r en segundo dígito
    displayBuffer[(comOffset + 2) * 2] = 0x50; // r en tercer dígito
  } else {
    // Extraer dígitos
    uint8_t hundreds = (number / 100) % 10;
    uint8_t tens = (number / 10) % 10;
    uint8_t units = number % 10;
    
    // Configurar cada dígito en el buffer
    // Solo mostrar centenas si es diferente de 0
    if (hundreds > 0 || number >= 100) {
      displayBuffer[comOffset * 2] = digitTable[hundreds];
    } else {
      displayBuffer[comOffset * 2] = 0x00; // Apagar centenas si es 0
    }
    
    // Solo mostrar decenas si el número es >= 10 o si hay centenas
    if (tens > 0 || number >= 10) {
      displayBuffer[(comOffset + 1) * 2] = digitTable[tens];
    } else if (number >= 100) {
      displayBuffer[(comOffset + 1) * 2] = digitTable[0]; // Mostrar 0 en decenas si hay centenas
    } else {
      displayBuffer[(comOffset + 1) * 2] = 0x00; // Apagar decenas si es 0
    }
    
    displayBuffer[(comOffset + 2) * 2] = digitTable[units]; // Siempre mostrar unidades
  }
}

// Función para mostrar un patrón de test en ambos displays
void showTestPattern(uint8_t pattern) {
  for (int display = 0; display < 2; display++) {
    uint8_t comOffset = display * 3;
    for (int digit = 0; digit < 3; digit++) {
      displayBuffer[(comOffset + digit) * 2] = pattern;
    }
  }
  updateDisplay();
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
  
  Serial.println("=== HT16K33 Dual Display Test ===");
  Serial.println("Iniciando prueba de displays con un solo HT16K33...");
  
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
  Serial.println("Test inicial: Encendiendo todos los segmentos...");
  showTestPattern(0xFF);
  delay(2000);
  
  // Limpiar displays
  clearDisplays();
  delay(500);
  
  Serial.println("Iniciando secuencia de prueba...");
  Serial.println("Secuencia:");
  Serial.println("1. Contador 0-99 en ambos displays");
  Serial.println("2. Números diferentes en cada display");
  Serial.println("3. Dígitos hexadecimales");
  Serial.println("4. Test de segmentos");
  Serial.println();
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = currentTime;
    
    // Prueba 1: Contador 0-99 en ambos displays
    if (counter < 100) {
      showNumber(0, counter);  // Display 1
      showNumber(1, counter);  // Display 2
      updateDisplay();
      Serial.print("Contador: ");
      Serial.println(counter);
    }
    // Prueba 2: Números diferentes en cada display
    else if (counter < 200) {
      int num1 = counter - 100;
      int num2 = 99 - num1;
      showNumber(0, num1);   // Display 1
      showNumber(1, num2);   // Display 2
      updateDisplay();
      Serial.print("Display 1: ");
      Serial.print(num1);
      Serial.print(", Display 2: ");
      Serial.println(num2);
    }
    // Prueba 3: Mostrar números incrementales
    else if (counter < 300) {
      int step = counter - 200;
      showNumber(0, step);      // Display 1: 0-99
      showNumber(1, step + 100); // Display 2: 100-199
      updateDisplay();
      Serial.print("Display 1: ");
      Serial.print(step);
      Serial.print(", Display 2: ");
      Serial.println(step + 100);
    }
    // Prueba 4: Test de todos los segmentos
    else if (counter < 310) {
      showTestPattern(0xFF);
      Serial.println("Test: Todos los segmentos encendidos");
    }
    // Prueba 5: Test de segmentos individuales
    else if (counter < 318) {
      int segmentTest = counter - 310;
      uint8_t pattern = 1 << segmentTest;
      showTestPattern(pattern);
      Serial.print("Test segmento: ");
      Serial.println(segmentTest);
    }
    // Reiniciar secuencia
    else {
      counter = -1;
      clearDisplays();
      Serial.println("\n=== Reiniciando secuencia de prueba ===\n");
    }
    
    counter++;
  }
  
  // Pequeña pausa para no saturar el loop
  delay(10);
}