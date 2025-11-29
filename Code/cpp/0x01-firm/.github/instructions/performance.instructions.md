---
applyTo: '**/*.cpp,**/*.h,**/*.hpp,**/*.ino'
description: 'Performance optimization guidelines for embedded systems'
---

# Embedded Performance Guidelines

## Memory Optimization
- Use appropriate data types (uint8_t vs uint32_t)
- Minimize global variable usage
- Prefer stack allocation over dynamic allocation
- Use PROGMEM for constants to save RAM
- Pack structures to minimize memory footprint

## CPU Optimization
- Minimize operations in interrupt service routines
- Use bit manipulation for efficient flag operations
- Cache frequently calculated values
- Avoid floating-point operations when possible
- Use lookup tables instead of complex calculations

## Real-Time Performance
- Design for predictable execution timing
- Minimize blocking operations in time-critical paths
- Use appropriate task scheduling strategies
- Implement efficient state machines for control flow
- Profile critical code sections for timing

## Power Optimization
- Use sleep modes when appropriate
- Minimize unnecessary peripheral usage
- Implement efficient polling strategies
- Turn off unused hardware modules
- Optimize clock speeds for power vs performance trade-offs

## I/O Optimization
- Batch I/O operations when possible
- Use interrupts instead of polling for events
- Implement efficient buffer management
- Minimize serial communication overhead
- Use DMA for large data transfers when available

## Code Size Optimization
- Avoid unnecessary library inclusions
- Use compiler optimization flags appropriately
- Implement efficient algorithms for space-constrained environments
- Share common code between modules
- Use function inlining judiciously