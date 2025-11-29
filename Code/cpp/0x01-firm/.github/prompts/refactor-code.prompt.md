---
agent: 'agent'
tools: ['codebase']
description: 'Refactor embedded C++ code for better performance and maintainability'
---

# Embedded Code Refactoring Assistant

Refactor embedded C++ code to improve performance, maintainability, and follow best practices while preserving functionality.

## Refactoring Goals

### Performance Optimization
- **Memory Efficiency**: Reduce RAM usage, optimize data structures
- **CPU Optimization**: Minimize cycles in critical paths
- **Power Efficiency**: Reduce unnecessary operations and power consumption
- **Real-Time Performance**: Ensure predictable timing behavior

### Code Quality Improvements
- **Modularity**: Break large functions into smaller, focused units
- **Abstraction**: Improve hardware abstraction layers
- **Reusability**: Extract common patterns into reusable components
- **Readability**: Improve naming and code organization

## Refactoring Strategies

### Embedded-Specific Patterns
- **State Machines**: Convert complex control logic to clear state machines
- **Hardware Abstraction**: Create interfaces for hardware dependencies
- **Memory Pools**: Replace dynamic allocation with fixed pools
- **Lookup Tables**: Replace calculations with pre-computed tables
- **Bit Manipulation**: Use efficient bit operations for flags and controls

### Code Organization
- **Single Responsibility**: Ensure functions have one clear purpose
- **Dependency Injection**: Make hardware dependencies configurable
- **Error Handling**: Implement consistent error handling patterns
- **Configuration**: Extract magic numbers to configuration constants

## Refactoring Process

1. **Analysis Phase**
   - Identify performance bottlenecks
   - Find code duplication and complexity
   - Locate embedded anti-patterns

2. **Planning Phase**
   - Prioritize refactoring tasks by impact
   - Ensure backward compatibility when needed
   - Plan incremental changes to minimize risk

3. **Implementation Phase**
   - Apply refactoring patterns systematically
   - Maintain existing functionality and behavior
   - Update tests and documentation accordingly

## Safety Guidelines
- Preserve existing behavior and interfaces
- Maintain real-time performance characteristics
- Keep memory usage within original bounds
- Ensure hardware timing requirements are met
- Update tests to reflect changes