<!-- Inspired by: https://github.com/github/awesome-copilot/blob/main/instructions/cmake-vcpkg.instructions.md -->
---
applyTo: '**/*.cpp,**/*.h,**/*.hpp,**/*.ino'
description: 'C++ embedded development guidelines for PlatformIO projects'
---

# C++ Embedded Development Guidelines

## Language Standards
- Use C++11 or newer features when supported by target platform
- Prefer Arduino-style C++ for compatibility
- Use standard library sparingly due to memory constraints
- Follow RAII principles where applicable

## Code Organization
- Separate interface declarations (.h) from implementations (.cpp)
- Use header guards or #pragma once
- Keep class definitions in headers, implementations in source files
- Group related functionality into classes or namespaces

## Memory Management
- Avoid dynamic allocation (new/delete, malloc/free)
- Use stack-based objects and arrays
- Prefer const references for parameter passing
- Use PROGMEM for string literals and large constants
- Be mindful of stack size limitations

## Arduino/PlatformIO Specific
- Use Arduino core functions appropriately (digitalRead, analogWrite, etc.)
- Implement setup() and loop() functions correctly
- Use appropriate delay functions (delay, delayMicroseconds)
- Handle interrupts properly with IRAM_ATTR when needed

## Performance Optimization
- Minimize floating-point operations
- Use bit manipulation for efficiency
- Cache frequently accessed values
- Avoid expensive operations in main loop
- Use appropriate data types (uint8_t, uint16_t, etc.)

## Hardware Integration
- Abstract hardware dependencies behind interfaces
- Use meaningful pin definitions from config.h
- Handle hardware initialization in setup()
- Implement proper error handling for hardware failures

## Debugging and Monitoring
- Use Serial.print() for debugging output
- Implement watchdog timer when appropriate
- Add LED indicators for system status
- Use assertions for critical condition checking