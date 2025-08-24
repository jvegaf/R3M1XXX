# TM1638 LCD/Encoders Test (ESP32-C3 + PlatformIO)

Este proyecto prueba la PCB `Encoders-LCD` basada en el IC TM1638 (driver de 7 segmentos + teclado) usando un ESP32-C3-DevKitM-1 con PlatformIO (Arduino framework).

## Hardware

- IC principal: TM1638 (U5)
- Displays: 2 módulos de 3 dígitos (GR1..GR6 pobladas)
- Teclado: Matriz de teclas a través de líneas Kx del TM1638
- Conector principal: J1 (JST-PH 1x7 horizontal)

### Pinout J1 (según KiCad)
- J1.1 = GND
- J1.2 = VCC
- J1.3 = SDA (I2C de encoders/TBD, no usado por TM1638)
- J1.4 = SCL (I2C de encoders/TBD, no usado por TM1638)
- J1.5 = DIO → TM1638 pin 26
- J1.6 = CLK → TM1638 pin 27
- J1.7 = STB → TM1638 pin 28

Nota: Verifica la tensión de VCC (3V3 o 5V) de tu placa. Ajusta la alimentación del ESP32-C3 y niveles lógicos según corresponda.

## Firmware

- Entorno: PlatformIO
- Board: `esp32-c3-devkitm-1`
- Framework: `arduino`

### Pines por defecto (configurables)
En `platformio.ini`:
```
build_flags =
  -D PIN_TM_STB=4
  -D PIN_TM_CLK=5
  -D PIN_TM_DIO=6
```
Cámbialos a los GPIO que uses para conectar STB/CLK/DIO a J1.5/J1.6/J1.7.

## Estructura
- `include/TM1638_Simple.hpp`: Driver mínimo por bit-bang (header-only)
- `src/main.cpp`: Herramienta de depuración manual por Serial para segmentos/LEDs:
  - Comandos:
    - `n`/`p`: siguiente/anterior segmento (A..G, DP) y aplica
    - `>`/`<`: siguiente/anterior dígito (0..5)
    - `0..5`: selecciona dígito directo
    - `a`: alterna aplicar a todos los dígitos
    - `l`: alterna LED del dígito actual (o todos si `a` activo)
    - `c`: limpia segmentos (conserva LEDs)
    - `h`: ayuda
  - Muestra continuamente cambios de teclado en formato `Keys raw: 0x???????? [b0 b1 b2 b3]`

## Compilar y cargar
```
pio run -e esp32-c3-devkitm-1
pio run -e esp32-c3-devkitm-1 -t upload
pio device monitor -b 115200
```

## Uso rápido en monitor serie
- Abre el monitor: `pio device monitor -b 115200`
- Teclea comandos (sin Enter necesario) y observa cambios:
  - Paso de segmentos con `n`/`p`, cambia dígito con `>`/`<` o `0..5`
  - Activa `a` para aplicar el segmento a todos los dígitos
  - `l` alterna LEDs; `c` limpia segmentos; `h` muestra ayuda
  - Al pulsar teclas físicas, aparecerá `Keys raw: ...`

## TM1638 (resumen de protocolo)
- Interfaz 3 hilos: STB, CLK, DIO (LSB primero)
- Comandos:
  - 0x40: escritura auto-incremento RAM de display (16 bytes)
  - 0x44: escritura dirección fija (usa 0xC0|addr)
  - 0x42: lectura teclado (lee 4 bytes)
  - 0xC0: base de dirección RAM
  - 0x88..0x8F: encendido y brillo (0..7)
- Segmentos por byte: bits 0..6 = A..G, bit7 = DP (asumido; confirmar con prueba de segmentos)

## Notas
- Este repositorio no fija versión de `platform = espressif32`; si usas APIs/flags modernos, fija versiones explícitas.
- Si inviertes dígitos o segmentos, adapta `encodeDigit()` o el orden de posiciones en el firmware.
- Si deseas mapeo de teclas legible, decodifica `readKeysRaw()` a Kx y GRx según tu matriz específica.

## Referencias
- TM1638 (Titan Microelectronics): sitios de datasheet públicos (consulta uno disponible en tu región)
  - https://datasheet39.com/PDF/971679/TM1638-datasheet.html
- KiCad del proyecto: `kicad/Encoders-LCD.kicad_sch` y `kicad/Encoders-LCD.net`