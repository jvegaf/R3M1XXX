---
applyTo: "**/platformio.ini,**/*.cpp,**/*.h,**/*.hpp,**/*.ino"
description: "PlatformIO development standards and best practices"
---

# PlatformIO Development Standards

## Project Configuration
- Keep `platformio.ini` configuration clean and well-documented
- Use environment-specific configurations for different targets or build modes
- Specify exact library versions to ensure reproducible builds
- Document any custom build flags or compiler options

## Library Management
- Prefer semantic versioning for library dependencies
- Use library registry IDs rather than Git URLs when possible
- Keep library dependencies minimal and well-justified
- Document library usage and hardware requirements in code comments

## Build Configuration
- Use appropriate optimization levels for development vs production builds
- Configure debug symbols for development environments
- Set up proper serial monitor configuration for debugging
- Consider memory layout and flash usage optimization

## Testing Framework Integration
- Use PlatformIO's native testing framework for unit tests
- Separate hardware-dependent tests from pure logic tests
- Implement mock objects for hardware interfaces during testing
- Set up continuous integration for automated testing

## Development Workflow
- Use version control with meaningful commit messages
- Tag releases with semantic versioning
- Maintain separate development and production configurations
- Document hardware setup and pin assignments

## Hardware Abstraction
- Create clear interfaces between hardware-specific and generic code
- Use consistent naming for GPIO pins and hardware resources
- Document hardware connections and wiring requirements
- Implement proper initialization sequences for hardware components

## Debugging and Monitoring
- Set up proper serial debugging with meaningful log levels
- Use debug builds with appropriate symbol information
- Implement runtime diagnostics for hardware health monitoring
- Consider remote debugging capabilities for deployed systems