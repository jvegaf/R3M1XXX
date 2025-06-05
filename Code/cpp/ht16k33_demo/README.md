# HT16K33 Dual Display Test

Este proyecto prueba dos módulos de display de 7 segmentos de 3 dígitos (modelo 3631AS) controlados por un solo chip HT16K33 conectado a una Raspberry Pi Pico mediante I2C.

## Hardware

- **Microcontrolador**: Raspberry Pi Pico
- **Controlador de Display**: 1x HT16K33 (dirección I2C: 0x70)
- **Displays**: 2x módulos de 3 dígitos de 7 segmentos (3631AS) - cátodo común
- **Comunicación**: I2C
  - SDA: GPIO 4 (Pin 6) - por defecto en arduino-mbed
  - SCL: GPIO 5 (Pin 7) - por defecto en arduino-mbed

## Configuración del Hardware

El HT16K33 controla ambos displays mediante multiplexación:
- **Display 1**: Conectado a COM0, COM1, COM2 del HT16K33
- **Display 2**: Conectado a COM3, COM4, COM5 del HT16K33
- Los segmentos a-g de ambos displays están conectados a las líneas ROW del HT16K33

## Funcionalidades del Programa

1. **Escaneo I2C**: Detecta automáticamente el HT16K33 en la dirección 0x70
2. **Test de inicialización**: Enciende todos los segmentos para verificar conexiones
3. **Secuencia de pruebas**:
   - Contador 0-99 en ambos displays
   - Números diferentes en cada display
   - Números incrementales (0-99 vs 100-199)
   - Test de todos los segmentos
   - Test de segmentos individuales
4. **Monitoreo serie**: Salida detallada por el puerto serie a 115200 baud

## Compilación y Carga

```bash
# Compilar el proyecto
pio run

# Cargar al microcontrolador
pio run --target upload

# Monitorear salida serie
pio device monitor
```

## Solución de Problemas

### El display no enciende
- Verificar conexiones I2C (SDA=GPIO4/Pin6, SCL=GPIO5/Pin7)
- Verificar alimentación del HT16K33 (3.3V/5V según diseño)
- Verificar que la dirección I2C sea 0x70

### Solo funciona un display
- Verificar conexiones de los pines COM3-COM5 para el segundo display
- Verificar conexiones de cátodos comunes

### Segmentos incorrectos
- Verificar la tabla de dígitos `digitTable[]` para displays cátodo común
- Verificar conexiones ROW0-ROW6 del HT16K33

## Configuración del Buffer HT16K33

El HT16K33 utiliza un buffer de 16 bytes donde cada byte representa una fila de la matriz:
- Bytes 0, 2, 4: Display 1 (dígitos 1, 2, 3)
- Bytes 6, 8, 10: Display 2 (dígitos 1, 2, 3)
- Los bytes impares se pueden usar para control adicional

## Personalización

Para cambiar la dirección I2C, modifica las constantes en `main.cpp`:

```cpp
#define HT16K33_ADDRESS 0x70  // Dirección I2C del HT16K33
```

**Nota**: Los pines I2C están fijados por el framework arduino-mbed:
- SDA: GPIO 4 (Pin 6)  
- SCL: GPIO 5 (Pin 7)
