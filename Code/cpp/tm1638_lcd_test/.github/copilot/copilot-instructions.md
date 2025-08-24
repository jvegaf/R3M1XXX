# Instrucciones para GitHub Copilot

## Guías Prioritarias

Al generar código para este repositorio:

1. Compatibilidad de Versiones: Detecta y respeta siempre las versiones exactas de lenguajes, frameworks y librerías usadas en este proyecto
2. Archivos de Contexto: Si existen, prioriza los patrones y estándares definidos en el directorio .github/copilot
3. Patrones de la Base de Código: Cuando los archivos de contexto no den una guía específica, escanea la base de código para identificar patrones establecidos
4. Consistencia Arquitectónica: Mantén nuestro estilo arquitectónico Monolítico y los límites establecidos
5. Calidad de Código: Prioriza Mantenibilidad en todo el código generado

### Resumen de tecnologías detectadas (solo a partir del repo)

- Gestor/entorno: PlatformIO
- Entorno activo: `[env:esp32-c3-devkitm-1]`
- Plataforma: `espressif32` (sin versión fijada en `platformio.ini`)
- Framework: `arduino`
- Placa: `esp32-c3-devkitm-1`
- Lenguaje: C/C++ (estándar no fijado en `platformio.ini`)
- Dependencias declaradas: No hay `lib_deps` definidos en `platformio.ini`
- Código fuente: `src/main.cpp` existe pero no contiene implementación aún

Nota: Evita suposiciones sobre versiones específicas del core de Arduino-ESP32, toolchain o C++ estándar. Si necesitas características de versiones concretas, fija versiones explícitas en `platformio.ini` y documenta el cambio.

## Detección de Versiones Tecnológicas

Antes de generar código, verifica:

1. Versiones de Lenguaje
   - Revisa flags de compilación y estándar C/C++ definidos (si existen) en `platformio.ini` (por ejemplo, `build_flags`)
   - No uses características del lenguaje más allá de lo que el toolchain por defecto de PlatformIO para este entorno soporte; cuando sea necesario, añade los flags de estándar pertinentes de forma explícita y documentada

2. Versiones de Frameworks
   - Identifica la versión efectiva del framework Arduino para ESP32 resuelta por PlatformIO (cuando el lock/manifest esté disponible). Si no está fijada en este repo, evita usar APIs que no sean ampliamente compatibles

3. Versiones de Librerías
   - Si agregas dependencias, decláralas y fíjalas en `platformio.ini` bajo `lib_deps` con versiones o commits concretos
   - Genera código compatible con las versiones específicas que declares

## Archivos de Contexto

Prioriza los siguientes archivos en `.github/copilot` si existen:

- architecture.md: Guías de arquitectura del sistema
- tech-stack.md: Detalles de versiones tecnológicas y frameworks
- coding-standards.md: Estándares de estilo y formato de código
- folder-structure.md: Guías de organización del proyecto
- exemplars.md: Patrones de código ejemplares a seguir

## Instrucciones de Escaneo de la Base de Código

Cuando los archivos de contexto no den una guía específica:

1. Identifica archivos similares al que se modifica o crea
2. Analiza patrones de:
   - Convenciones de nombres
   - Organización de código
   - Manejo de errores
   - Estilo de documentación
   - Patrones de pruebas
3. Sigue los patrones más consistentes encontrados en la base de código
4. Cuando existan patrones conflictivos, prioriza los de archivos más nuevos
5. Nunca introduzcas patrones que no existan en la base de código

## Estándares de Calidad de Código

### Mantenibilidad
- Escribe código auto-documentado con nombres claros
- Sigue las convenciones de nombres y organización evidentes en la base de código; si aún no existen, mantén un estilo coherente y simple y documenta decisiones relevantes en este directorio
- Mantén funciones enfocadas en responsabilidades únicas
- Limita la complejidad y longitud de funciones; favorece pequeñas utilidades reutilizables en `src/` o `lib/`
- Centraliza constantes de pines y configuración de periféricos en headers dentro de `include/` cuando aporte claridad

## Requisitos de Documentación

Nivel: Mínima

- Igualar el nivel y estilo de comentarios encontrados en el código existente (cuando se agregue)
- Documentar solo comportamientos no obvios (por ejemplo, timings, requisitos eléctricos, orden de inicialización)
- Usar comentarios breves junto a declaraciones de pines, interrupciones y configuraciones de buses

## Enfoque de Pruebas

### Pruebas Unitarias
- Ubica pruebas en `test/` conforme a la convención de PlatformIO cuando se incorporen
- Mantén nombres de casos de prueba descriptivos y pequeños
- Aísla dependencias de hardware real; si no hay patrón local, comenta claramente los requisitos de hardware para cualquier prueba que lo necesite

## Guías de Control de Versiones

Tipo: Personalizada

- Igualar el patrón de versionado observado en el proyecto (no hay uno declarado actualmente)
- Si introduces versionado o changelog, documenta el formato en `.github/copilot/tech-stack.md` o un archivo de documentación dedicado

## Mejores Prácticas Generales

- Usa nombres de pines y constantes simbólicas en lugar de valores mágicos
- Evita dependencias implícitas; declara librerías en `platformio.ini` con `lib_deps` y versiones fijas
- Manejo de errores: valida retornos de inicialización de periféricos y registra/indica fallos de forma consistente (por ejemplo, LED de estado o logs seriales en modo debug)
- Mantén la configuración específica de la placa agrupada (por ejemplo, en `include/board_pins.h`) si la complejidad crece
- No introduzcas características del lenguaje o APIs que requieran cambiar la plataforma sin anotar y fijar dichas versiones

## Guía Específica del Proyecto

- Escanea la base de código antes de generar cualquier archivo nuevo
- Respeta los límites arquitectónicos actuales (aplicación monolítica embebida sobre Arduino/ESP32)
- Igualar el estilo y patrones del código circundante cuando este exista
- En caso de duda, prioriza la consistencia con la configuración actual de PlatformIO sobre buenas prácticas externas o características más nuevas del lenguaje

### Ejemplos mínimos basados en la estructura actual

Actualmente no hay ejemplos en la base de código. Cuando se añada implementación en `src/main.cpp` u otros archivos, incorpora aquí ejemplos concretos extraídos de ese código para guiar a Copilot.

Nota final: Este documento se basa únicamente en los archivos presentes en el repositorio en el momento de su creación. Actualízalo cuando se fijen versiones, se agreguen dependencias o se establezcan patrones de código adicionales.