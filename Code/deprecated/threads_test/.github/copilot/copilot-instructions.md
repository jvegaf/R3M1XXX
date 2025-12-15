# Instrucciones para GitHub Copilot

## Guías Prioritarias

Al generar código para este repositorio:

1. Compatibilidad de Versiones: Detecta y respeta siempre las versiones exactas de lenguajes, frameworks y librerías usadas en este proyecto
2. Archivos de Contexto: Prioriza los patrones y estándares definidos en este archivo y en esta base de código (no existe directorio .github/copilot previo adicional)
3. Patrones de la Base de Código: Cuando no haya guía específica, analiza los archivos de `src/` e `include/` para identificar patrones existentes
4. Consistencia Arquitectónica: Mantén el estilo monolítico por capas sencillo típico de proyectos Arduino/PlatformIO
5. Calidad de Código: Prioriza mantenibilidad y testabilidad en el código generado, evitando complejidad innecesaria

## Detección de Versiones Tecnológicas

Basado en los artefactos del proyecto:

- PlatformIO: plataforma espressif32; framework Arduino; board esp32-c3-devkitm-1
- C++ estándar de compilación: gnu++11 (sin RTTI y con excepciones habilitadas pero evitar su uso)
- Toolchain: riscv32-esp-elf-g++ para ESP32-C3
- ESP-IDF subyacente: v4.4.7 (según defines IDF_VER en compile_commands.json)

Requisitos:
- No uses características de C++ posteriores a C++11
- Evita `new`/`delete` y asignaciones dinámicas innecesarias; prioriza stack/estático
- No dependas de RTTI (typeid, dynamic_cast) ni de excepciones personalizadas
- Usa solo APIs disponibles en Arduino-ESP32 compatibles con IDF 4.4.x

## Archivos de Contexto

Prioriza patrones presentes en:
- `src/main.cpp`, `src/mux.cpp`, `src/mux.h`, `include/pins.h`

## Instrucciones de Escaneo de la Base de Código

Patrones observados y a seguir:
- Convenciones de nombres:
  - Clases: PascalCase (por ejemplo, `Mux`)
  - Métodos/funciones: lowerCamelCase (`begin`, `setMuxChannel`, `read`)
  - Constantes y pines: UPPER_SNAKE_CASE (`MUX_SIG`, `MUX_S0`)
- Organización de código:
  - Headers con `#pragma once`
  - Separación `.h/.cpp` para clases
  - `include/` para pines y constantes compartidas
- Manejo de E/S y logging:
  - Uso de `Serial.begin`, `Serial.print/println` para mensajes
- Manejo de errores:
  - No se usan excepciones; preferir retornos booleanos/códigos de estado si se amplía
- Estilo de documentación:
  - Comentarios escuetos en línea; sin Doxygen/JSDoc
- Pruebas:
  - No hay tests aún; si se agregan, usar estructura de PlatformIO `test/` y Unity (incluido por IDF)

Reglas adicionales:
- Evita introducir frameworks de logging distintos; usa `Serial` como en el código existente
- Mantén funciones cortas y con responsabilidad única
- Sigue el patrón de callbacks con punteros a función para notificaciones (`void (*)(uint8_t)`). Si se proponen lambdas, que sean compatibles con C++11 y convertibles a puntero de función sin capturas

## Estándares de Calidad de Código

### Mantenibilidad
- Nombres claros y consistentes con lo existente
- Limitar complejidad; bucles simples y sin lógica anidada excesiva
- Inicializaciones en el constructor y uso de `const` donde aplique

### Rendimiento
- Minimiza asignaciones dinámicas y heap
- Evita trabajo pesado dentro de `loop()`; si es necesario, dividir en pasos
- Usa `millis()` para temporización no bloqueante; evita `delay()` en rutas críticas

### Seguridad
- Validar índices de canales de multiplexor (0..15)
- Configurar modos de pin explícitamente (`pinMode`)

### Testabilidad
- Extraer lógica en funciones/métodos puros cuando sea posible
- Diseñar APIs que permitan inyectar callbacks

## Requisitos de Documentación
- Comentarios breves para comportamientos no obvios (p. ej., debouncing con `millis()`)
- Documentar parámetros y rangos cuando se amplíen APIs

## Enfoque de Pruebas
- Si se agregan pruebas, usar PlatformIO Test Runner con Unity en `test/`
- Estilo: asserts simples, pruebas unitarias por clase o módulo

## Guías Específicas por Tecnología

### Arduino/ESP32 (C++11)
- Usar solo funciones de Arduino disponibles en el core ESP32C3 actual
- Evitar dependencias externas no listadas en `platformio.ini`
- Mantener compatibilidad con `-std=gnu++11` y flags de compilación actuales

## Guías de Control de Versiones
- No se prescribe esquema específico; usar mensajes de commit claros y referenciar archivos modificados

## Mejores Prácticas Generales
- Mantener el patrón de directorios PlatformIO (`src/`, `include/`, `lib/`, `test/`)
- Manejo de errores por códigos de retorno o `bool` cuando aplique
- Logging con `Serial` únicamente
- Configuración a través de headers en `include/` para pines y constantes

## Guía Específica del Proyecto
- Respeta los límites y estilo simples actuales
- Prioriza consistencia con el código existente sobre características nuevas
- Cuando existan dudas, replica el estilo de `Mux` y `main.cpp`
