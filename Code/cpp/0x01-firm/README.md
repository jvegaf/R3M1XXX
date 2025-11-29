# r3m1xxx 0x01 DJ MIDI Controller

Este repositorio contiene el firmware para un controlador DJ MIDI profesional basado en la **Raspberry Pi Pico (RP2040)**.

El proyecto está diseñado con una arquitectura de **baja latencia** y **multiprocesamiento (Dual-Core)**, separando la lógica crítica de comunicación MIDI de las tareas intensivas de renderizado visual (LEDs), garantizando una experiencia de usuario fluida y sin bloqueos (jitter-free).

## 📚 Documentación

La documentación completa se encuentra en la carpeta `docs/`:

- **[Guía de Hardware](docs/HARDWARE.md)**: Diagramas de pines, direcciones I2C y cableado.
- **[Referencia de API](docs/API.md)**: Documentación de las clases y métodos del firmware.
- **[Guía de Desarrollador](docs/DEVELOPER.md)**: Arquitectura del sistema, compilación y testing.

## 🚀 Inicio Rápido

### Requisitos
- VS Code con extensión **PlatformIO**.
- Hardware basado en RP2040.

### Instalación
1. Clonar el repositorio.
2. Abrir en VS Code.
3. Conectar la Pico en modo Bootloader (mantener BOOTSEL).
4. Ejecutar **PlatformIO: Upload**.

## ⚙️ Configuración Básica
Edita `include/config.h` para cambiar canales MIDI o pines:

```cpp
#define MIDI_CH          1
#define CC_POTS_START    10
#define NOTE_MAIN_START  36
```

## 🏗 Arquitectura Dual-Core

- **Core 0**: Dedicado a MIDI e I2C (1MHz). Prioridad máxima.
- **Core 1**: Dedicado a animaciones LED (NeoPixel).

Para más detalles técnicos, consulta la [Guía de Desarrollador](docs/DEVELOPER.md).
