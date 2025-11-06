<!-- Inspired by: https://github.com/github/awesome-copilot/blob/main/instructions/cmake-vcpkg.instructions.md -->
---
applyTo: "**/*.cpp,**/*.h,**/*.hpp,**/*.ino"
description: "C++ embedded development best practices for PlatformIO and Arduino framework"
---

# C++ Embedded Development Guidelines

## Modern C++ Standards for Embedded
- Use C++11/C++14 features appropriately, considering memory constraints
- Prefer `constexpr` for compile-time constants to reduce runtime memory usage
- Use `auto` judiciously to improve code readability without sacrificing performance
- Employ RAII principles for resource management, especially for hardware interfaces

## Memory Management
- Minimize dynamic memory allocation (avoid `new`/`delete` in favor of stack allocation)
- Use fixed-size arrays and containers when possible
- Prefer `std::array` over C-style arrays for bounds checking
- Consider memory pool patterns for predictable allocation needs

## Hardware Interface Best Practices
- Always check return values from hardware communication functions (I2C, SPI, UART)
- Use timeout patterns for hardware operations to prevent infinite blocking
- Implement proper error handling for communication failures
- Document hardware timing requirements and constraints in code comments

## Performance Considerations
- Minimize expensive operations in interrupt service routines (ISRs)
- Use appropriate data types (prefer `uint8_t`, `uint16_t` for embedded efficiency)
- Consider loop unrolling and compile-time optimizations for critical paths
- Profile memory usage and stack depth in resource-constrained environments

## Concurrency and Real-time Behavior
- Use volatile keyword for variables accessed in interrupt context
- Implement proper critical sections for shared resource access
- Consider deterministic execution times for real-time requirements
- Document interrupt priorities and timing assumptions

## Error Handling
- Use return codes rather than exceptions in resource-constrained environments
- Implement defensive programming practices with input validation
- Provide clear error reporting mechanisms for debugging
- Consider watchdog timer integration for fault recovery

## Code Organization
- Separate hardware abstraction layer (HAL) from application logic
- Use header guards or `#pragma once` consistently
- Keep hardware-specific code isolated and well-documented
- Follow consistent naming conventions for registers and hardware resources