# HT16K33 Embedded System Project - Copilot Instructions

## Project Overview

This is a **C++ embedded systems project** using **PlatformIO** and **Arduino framework** for **Raspberry Pi Pico**, specifically designed to interface with the **HT16K33 LED matrix and keyboard controller**.

## Project Structure
- **Platform**: Raspberry Pi Pico (RP2040)
- **Framework**: Arduino
- **Build System**: PlatformIO
- **Target**: Embedded I2C device communication
- **Main Library**: HT16K33 LED/keyboard controller

## Development Guidelines

Apply the specific coding standards from our instruction files:
- [C++ embedded development guidelines](./instructions/cpp-embedded.instructions.md)
- [PlatformIO development standards](./instructions/platformio.instructions.md)
- [Testing standards for embedded systems](./instructions/testing.instructions.md)
- [Documentation requirements](./instructions/documentation.instructions.md)
- [Performance optimization for embedded](./instructions/performance.instructions.md)
- [Security best practices](./instructions/security.instructions.md)

## Code Style Preferences

- **Naming**: Use descriptive variable names with camelCase for variables, PascalCase for classes
- **Memory Management**: Prefer stack allocation, minimize dynamic allocation
- **Error Handling**: Use return codes and defensive programming practices
- **Comments**: Focus on explaining _why_ not _what_, especially for timing-critical or hardware-specific code
- **Constants**: Use `const` and `constexpr` appropriately, define magic numbers as named constants

## Hardware-Specific Considerations

- **I2C Communication**: Always check return values from I2C operations
- **Timing**: Be mindful of delay() usage and hardware timing requirements
- **Memory Constraints**: Raspberry Pi Pico has limited RAM (264KB), optimize memory usage
- **Power Management**: Consider sleep modes and power consumption
- **Pin Usage**: Document pin assignments and avoid conflicts

## Testing Strategy

- **Unit Tests**: Focus on logic functions that can be tested independently
- **Hardware Integration Tests**: Test I2C communication and LED/keyboard functionality
- **Simulation**: Use PlatformIO's testing framework where possible
- **Hardware-in-the-Loop**: Test on actual hardware for timing-critical functions

## Documentation Standards

- **Code Comments**: Explain hardware interactions, timing requirements, and protocol details
- **README Updates**: Keep documentation current with pin assignments and setup instructions
- **Wiring Diagrams**: Include or reference circuit diagrams for hardware connections
- **API Documentation**: Document public functions with parameters and expected behavior