# HT16K33 LED & Keyboard Position Mapper

## Descripción

Herramienta de diagnóstico avanzada para mapear e identificar las posiciones físicas de LEDs y botones de teclado en dispositivos basados en el IC HT16K33.

## Características

### Tests de LEDs
- **Test Secuencial**: Prueba todos los 128 LEDs uno por uno mostrando información detallada
- **Test por Filas**: Enciende y prueba LEDs fila por fila
- **Test por Columnas**: Enciende y prueba LEDs columna por columna  
- **Test de Patrones**: Muestra patrones predefinidos (tablero de ajedrez, diagonales, marco)
- **Test Individual**: Permite probar un LED específico por su número
- **Vista de Grid**: Muestra el estado actual de todos los LEDs en formato visual

### Tests de Teclado
- **Mapeo de Teclado**: Detecta y muestra información de cada tecla presionada
- **Datos Raw**: Muestra los datos binarios y hexadecimales del scan del teclado
- **Modo Interactivo**: Combina test de LEDs con detección de teclas (pausa/resume)

## Información de Hardware

### HT16K33 LED Driver
- **Matriz LED**: 16 filas × 8 columnas = 128 LEDs
- **Teclado**: 3 grupos × 13 teclas = 39 teclas máximo
- **Dirección I2C Base**: 0x70 (configurable 0x70-0x77)
- **Comunicación**: I2C/TWI

### Raspberry Pi Pico
- **Pines I2C**:
  - SDA: GPIO 4
  - SCL: GPIO 5
- **UART** (para monitor serial):
  - TX: GPIO 0
  - RX: GPIO 1
- **Baudrate**: 57600

## Formato de Salida

### Información de LED
```
LED # 45 | Row:  5, Col: 5 | RAM[ 5]:bit5 | 0x20
```
- **LED #**: Número de LED (0-127)
- **Row**: Fila en la matriz (0-15)
- **Col**: Columna en la matriz (0-7)
- **RAM[byte]:bit**: Posición en la memoria RAM del HT16K33
- **Bitmask**: Máscara de bits en hexadecimal

### Información de Tecla
```
KEY PRESSED: # 7 | Group: 0, Position:  6 | [PRESSED]
KEY RELEASED: # 7 | [RELEASED]
```
- **Key #**: Número de tecla (1-39)
- **Group**: Grupo del teclado (0-2)
- **Position**: Posición dentro del grupo (0-12)
- **Estado**: PRESSED o RELEASED

### Datos Raw del Teclado
```
=== Raw Keyboard Data ===
Group 0: 0x0040 | Binary: 0 0000 0100 0000
Group 1: 0x0000 | Binary: 0 0000 0000 0000
Group 2: 0x0000 | Binary: 0 0000 0000 0000
```

## Uso

### 1. Compilar y Cargar

```bash
cd /path/to/ht16k33_test
pio run -t upload
```

### 2. Abrir Monitor Serial

```bash
pio device monitor
```

o usar tu terminal serial favorito a 57600 baudios.

### 3. Navegación del Menú

Al iniciar, verás el menú principal:

```
╔═══════════════════════════════════════════╗
║       HT16K33 Diagnostic Menu             ║
╠═══════════════════════════════════════════╣
║  LED Tests:                               ║
║    1 - Sequential LED Test (all 128)      ║
║    2 - Test LEDs by Row                   ║
║    3 - Test LEDs by Column                ║
║    4 - Pattern Test (checkerboard, etc)   ║
║    5 - Individual LED Test                ║
║    6 - LED Grid View                      ║
║                                           ║
║  Keyboard Tests:                          ║
║    7 - Keyboard Mapping                   ║
║    8 - Raw Keyboard Data                  ║
║                                           ║
║  Combined:                                ║
║    9 - Interactive Mode (LED + Keys)      ║
║    0 - Show this menu                     ║
╚═══════════════════════════════════════════╝
```

Ingresa un número (0-9) para seleccionar el modo de prueba.

### 4. Modos de Prueba

#### Opción 1: Test Secuencial de LEDs
- Enciende cada LED del 0 al 127 secuencialmente
- Muestra información detallada de cada LED
- Envía cualquier dato serial para abortar

#### Opción 2: Test por Filas
- Enciende una fila completa a la vez
- Útil para identificar el cableado de filas

#### Opción 3: Test por Columnas
- Enciende una columna completa a la vez
- Útil para identificar el cableado de columnas

#### Opción 4: Test de Patrones
- Muestra 4 patrones diferentes:
  1. Tablero de ajedrez
  2. Tablero inverso
  3. Rayas diagonales
  4. Marco de borde

#### Opción 5: Test Individual de LED
- Permite ingresar un número de LED (0-127)
- Enciende solo ese LED
- Útil para verificar posiciones específicas
- Ingresa 255 para salir

#### Opción 6: Vista de Grid
- Muestra el estado actual de todos los LEDs
- Formato visual con █ (encendido) y · (apagado)

#### Opción 7: Mapeo de Teclado
- Presiona teclas para ver su número y grupo
- Muestra eventos de presión y liberación
- Envía dato serial para salir

#### Opción 8: Datos Raw del Teclado
- Muestra continuamente los 3 registros de 16 bits
- Formato hexadecimal y binario
- Útil para depuración de bajo nivel

#### Opción 9: Modo Interactivo
- Combina test de LEDs con detección de teclas
- Presiona cualquier tecla para pausar/reanudar
- Ideal para pruebas integrales

## Mapeo de Hardware

### Cálculo de Posición LED

```cpp
LED_Number = Row × 8 + Column
Row = LED_Number / 8
Column = LED_Number % 8
```

### Estructura de Memoria RAM

El HT16K33 tiene 16 bytes de RAM para LEDs:
- `RAM[0]` controla LEDs 0-7 (Row 0)
- `RAM[1]` controla LEDs 8-15 (Row 1)
- ...
- `RAM[15]` controla LEDs 120-127 (Row 15)

Cada bit en el byte corresponde a una columna:
- bit 0 = Column 0
- bit 1 = Column 1
- ...
- bit 7 = Column 7

### Estructura de Teclado

39 teclas organizadas en 3 grupos de 13 teclas:
- **Group 0**: Teclas 1-13
- **Group 1**: Teclas 14-26
- **Group 2**: Teclas 27-39

## Solución de Problemas

### No se encienden LEDs
1. Verificar conexiones I2C (SDA, SCL)
2. Verificar dirección I2C (por defecto 0x70)
3. Verificar alimentación del HT16K33
4. Revisar salida del test de flash inicial

### No se detectan teclas
1. Verificar conexiones de la matriz de teclado
2. Verificar que las teclas estén correctamente conectadas a los pines KS
3. Usar Opción 8 para ver datos raw y confirmar actividad

### Comunicación I2C falla
1. Verificar pull-ups en líneas I2C (típicamente 4.7kΩ)
2. Verificar velocidad de bus I2C
3. Probar con diferentes direcciones I2C si hay conflictos

## Librería Utilizada

- **Nombre**: simple ht16k33 library
- **Autor**: Peter Sjoberg
- **Repositorio**: lpaseen/simple ht16k33 library
- **Licencia**: GPLv3

## Configuración PlatformIO

```ini
[env:pico]
platform = raspberrypi
board = pico
framework = arduino
lib_deps = 
    lpaseen/simple ht16k33 library@^1.1.7
monitor_speed = 57600
```

## Estructura del Proyecto

```
ht16k33_test/
├── src/
│   └── main.cpp          # Código principal de la herramienta
├── include/              # Headers personalizados (si los hay)
├── lib/                  # Librerías locales
├── test/                 # Tests unitarios
├── platformio.ini        # Configuración de PlatformIO
└── README.md            # Este archivo
```

## Constantes Configurables

En `main.cpp` puedes ajustar:

```cpp
constexpr uint16_t LED_DELAY_MS = 300;      // Delay entre tests de LED
constexpr uint16_t LED_FAST_DELAY_MS = 100; // Delay rápido
constexpr uint16_t SCAN_DELAY_MS = 50;      // Tasa de escaneo de teclado
constexpr uint16_t DEBOUNCE_MS = 200;       // Delay de anti-rebote
```

## Mejoras Futuras

- [ ] Exportar mapeo a archivo JSON
- [ ] Importar configuración de mapeo personalizado
- [ ] Soporte para múltiples HT16K33 en el mismo bus
- [ ] Animaciones de LED más complejas
- [ ] Detección de combinaciones de teclas
- [ ] Interfaz web para visualización

## Contribuciones

Este código sigue las mejores prácticas de sistemas embebidos:
- Uso eficiente de memoria
- Sin asignación dinámica
- Tipos de datos apropiados para sistemas embebidos
- Documentación clara
- Manejo de errores defensivo

## Licencia

Este proyecto hereda la licencia GPLv3 de la librería HT16K33 utilizada.

## Autor

Herramienta de diagnóstico para sistemas embebidos
Versión 2.0 - Noviembre 2025

---

**¿Encontraste un problema o tienes una sugerencia?** Abre un issue en el repositorio.
