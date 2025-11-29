---
description: 'Main GitHub Copilot instructions for C++ PlatformIO embedded firmware project'
---

# GitHub Copilot Instructions for 0x01-firm Project

## Project Overview
This is a C++ embedded firmware project using PlatformIO for Arduino/microcontroller development. The project focuses on matrix management and potentiometer control functionality.

## General Development Guidelines

### Code Style and Standards
- Use consistent C++ naming conventions (camelCase for variables, PascalCase for classes)
- Follow Arduino/embedded development best practices
- Prefer const correctness and minimal memory usage
- Use descriptive variable and function names
- Keep functions small and focused on single responsibility

### Project Structure
- `src/` - Main source code files
- `include/` - Header files and project configuration
- `lib/` - Project-specific libraries
- `test/` - Unit tests and testing code
- `platformio.ini` - PlatformIO configuration

### Memory Management
- Be mindful of RAM limitations on microcontrollers
- Avoid dynamic memory allocation when possible
- Use PROGMEM for storing constants in flash memory
- Prefer stack allocation over heap allocation

### Hardware Abstraction
- Use proper pin definitions from config.h
- Abstract hardware-specific code into separate modules
- Follow the existing MatrixManager and PotManager patterns

### Documentation
- Include brief comments for complex algorithms
- Document pin assignments and hardware connections
- Use Doxygen-style comments for public APIs
- Maintain README with setup and usage instructions

## File-Specific Guidelines
Refer to the specific instruction files in `.github/instructions/` for detailed guidelines on different aspects of development.

## Testing
- Write unit tests for core functionality
- Test on actual hardware when possible
- Use serial output for debugging embedded code
- Consider using PlatformIO's testing framework

## Performance Considerations
- Optimize for power consumption when applicable
- Minimize interrupt service routine (ISR) execution time
- Use appropriate data types to save memory
- Consider loop timing and real-time constraints