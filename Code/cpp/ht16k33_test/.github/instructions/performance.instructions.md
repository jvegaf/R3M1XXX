<!-- Based on: https://github.com/github/awesome-copilot/blob/main/instructions/performance-optimization.instructions.md -->
---
applyTo: "**/*.cpp,**/*.h,**/*.hpp,**/*.ino"
description: "Performance optimization guidelines for embedded systems"
---

# Performance Optimization for Embedded Systems

## Memory Optimization
- Minimize RAM usage through efficient data structures and stack allocation
- Use appropriate data types (uint8_t, uint16_t) to reduce memory footprint
- Eliminate memory leaks and unnecessary dynamic allocation
- Consider memory alignment for optimal access patterns

## CPU Performance
- Optimize critical loops and frequently called functions
- Use compile-time constants and constexpr where possible
- Minimize expensive operations in interrupt service routines
- Consider loop unrolling for performance-critical code paths

## Power Optimization
- Use sleep modes and power management features appropriately
- Minimize active processing time through efficient algorithms
- Consider clock scaling for power-sensitive applications
- Implement efficient polling strategies to reduce power consumption

## I/O Performance
- Batch I2C operations to reduce communication overhead
- Use appropriate communication speeds for hardware capabilities
- Implement efficient buffering strategies for data transfer
- Consider DMA for high-throughput data operations

## Real-Time Performance
- Ensure deterministic execution times for time-critical functions
- Minimize interrupt latency and jitter
- Use appropriate task scheduling and priority management
- Document timing requirements and constraints

## Code Size Optimization
- Use compiler optimization flags appropriately
- Eliminate dead code and unused functions
- Consider function inlining for small, frequently called functions
- Optimize string and constant data storage

## Profiling and Measurement
- Measure actual performance rather than assuming bottlenecks
- Use timing analysis tools for critical paths
- Monitor memory usage and stack depth
- Profile power consumption for battery-powered applications