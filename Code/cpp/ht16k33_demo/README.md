# HT16K33 Dual Display Utility (Diagnóstico + Demo)

Proyecto para Raspberry Pi Pico que controla **dos módulos 7 segmentos de 3 dígitos** con **un solo HT16K33** (I2C 0x70). Incluye:

- Modo diagnóstico interactivo (calibrar mapeo de segmentos)
- Modo demo (contador y patrones)
- Comandos serie para cambiar modo, velocidad, números, inversión y mapping en caliente

## Hardware

- MCU: Raspberry Pi Pico
- Driver: HT16K33 @ 0x70
- Displays: 2 × 3 dígitos, cátodo común (3631AS)
- I2C pico: SDA=GPIO4 (Pin 6), SCL=GPIO5 (Pin 7)
- I2C esp32 c3 super mini: SDA=GPIO8 SCL=GPIO9

Multiplexación:

- Display 0: COM0/COM1/COM2
- Display 1: COM3/COM4/COM5
- Segmentos (a..g, dp) comparten ROWx

## Funcionalidades

### 1. Modo Diagnóstico (default)

Rota un segmento lógico (a,b,c,d,e,f,g[,dp]) cada segundo para comprobar cableado.
Permite:

- Encender bits físicos concretos: `raw <n>`
- Registrar mapeo lógico→físico: `phys <bit>`
- Ajustar mapping en vivo: `setmap L P`
- Ver resumen y plantilla: `map`

### 2. Modo Demo

Secuencia automática: pares iguales 0–99, pares invertidos, rangos 0–99 vs 100–199, todos los segmentos, y barrido por segmento.
Puedes fijar números arbitrarios (`num`) y cambiar velocidad (`speed`).

### 3. Escaneo I2C

Confirma presencia del HT16K33 (0x70) al iniciar.

### 4. Comandos Serie

Interfaz interactiva a 115200 baud (lista completa abajo).

## Compilación y Carga

```bash
pio run              # compilar
pio run --target upload  # cargar
pio device monitor   # monitor serie (115200)
```

## Comandos Serie

| Comando          | Descripción                                    |
| ---------------- | ---------------------------------------------- |
| `help`           | Lista de comandos                              |
| `mode diag`      | Cambia a modo diagnóstico                      |
| `mode demo`      | Cambia a modo demo                             |
| `n` / `next`     | Siguiente segmento (diagnóstico)               |
| `p` / `prev`     | Segmento anterior                              |
| `a` / `auto`     | Alterna avance automático/manual               |
| `inv` / `invert` | Invierte lógica (active high/low)              |
| `show`           | Repite segmento actual                         |
| `raw <bit>`      | Enciende un bit físico (0–7)                   |
| `phys <bit>`     | Asocia bit físico al segmento lógico actual    |
| `setmap <L> <P>` | Fuerza segmentMap[L]=P (L=a..g/dp índice 0–7)  |
| `map`            | Muestra mapping actual y sugerido              |
| `resetmap`       | Limpia mapeo observado                         |
| `num A B`        | (demo) Muestra A en display 0 y B en display 1 |
| `speed ms`       | (demo) Ajusta intervalo (≥50 ms)               |
| `resetdemo`      | Reinicia secuencia demo                        |

## Procedimiento de Calibración de Mapping

1. Arranca (ya en modo diag).
2. Ejecuta `raw 0` .. `raw 7` y anota qué segmento lógico se ilumina.
3. Posiciónate en el segmento lógico correcto (comandos `next`/`prev`).
4. Usa `phys <bit>` para registrar el bit físico observado.
5. Repite hasta cubrir a..g (y dp si aplica).
6. Ejecuta `map` y copia el array sugerido en el código si deseas fijarlo permanentemente.

## Solución de Problemas

### Nada aparece al arrancar

1. Confirmar alimentación y masa comunes.
2. Revisar salida del escaneo I2C: debe detectar 0x70.
3. Si no detecta: comprobar pull-ups y continuidad SDA/SCL.

### Segmentos mezclados

Seguir el procedimiento de calibración y actualizar `segmentMap`.

### Segmentos invertidos (todo iluminado excepto el objetivo)

Usar `invert` (cambia `segmentsActiveHigh`).

### Solo un display responde

Revisar conexiones COM3–COM5 y soldaduras.

### Ghosting / Artefactos

- Reducir brillo (`HT16K33_CMD_BRIGHTNESS | n`, n=0..15)
- Cables más cortos / mejor alimentación.

## Diseño del Buffer

Se escriben 16 bytes iniciando en 0x00. Usamos sólo índices pares:

- Display 0: buffer[0], buffer[2], buffer[4]
- Display 1: buffer[6], buffer[8], buffer[10]
  Los impares quedan en 0.

## Personalización Rápida

Dirección I2C:

```cpp
#define HT16K33_ADDRESS 0x70
```

Mapping segmentos (en `main.cpp`):

```cpp
static uint8_t segmentMap[8] = {0,1,2,3,4,5,6,7}; // a,b,c,d,e,f,g,dp
```

Brillo (0–15) en init:

```cpp
writeCommand(HT16K33_ADDRESS, HT16K33_CMD_BRIGHTNESS | 8);
```

Velocidad demo (comando):

```
speed 250
```

I2C (arduino-mbed Pico): SDA GPIO4, SCL GPIO5.
