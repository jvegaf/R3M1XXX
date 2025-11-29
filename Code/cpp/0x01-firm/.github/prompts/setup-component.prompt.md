---
agent: 'agent'
tools: ['codebase']
description: 'Generate a new embedded C++ component for PlatformIO project'
---

# Embedded Component Generator

Your goal is to generate a new embedded C++ component following the project's patterns and embedded development best practices.

## Ask for Requirements
If not provided, ask for:
- Component name and purpose
- Hardware interface requirements (pins, communication protocols)
- Key functionality and methods needed
- Dependencies on other components

## Component Structure
Create components with:

### Header File (.h)
- Include guards or #pragma once
- Class declaration with public/private sections
- Constructor with hardware pin parameters
- Public methods for component interface
- Private member variables for state
- Hardware abstraction methods

### Implementation File (.cpp)
- Include corresponding header
- Constructor implementation with hardware initialization
- Public method implementations
- Private helper methods
- Proper error handling and validation

## Embedded Best Practices
- Use appropriate data types (uint8_t, uint16_t)
- Implement const correctness
- Minimize memory usage and dynamic allocation
- Include hardware initialization in constructor
- Add debug output using Serial when appropriate
- Follow existing MatrixManager/PotManager patterns

## Code Generation Guidelines
- Use consistent naming conventions (camelCase for variables, PascalCase for classes)
- Include brief documentation comments
- Add parameter validation
- Implement proper cleanup in destructors if needed
- Consider interrupt safety for shared data

## Integration
- Follow project structure (headers in include/, source in src/)
- Update config.h with new pin definitions if needed
- Consider integration with existing components
- Add to main.cpp setup() and loop() as appropriate