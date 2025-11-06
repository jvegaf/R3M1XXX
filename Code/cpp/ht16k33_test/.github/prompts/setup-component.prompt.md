---
mode: 'agent'
model: Claude Sonnet 4
tools: ['codebase', 'edit/editFiles', 'runCommands']
description: 'Setup a new embedded component or module for HT16K33 project'
---

# Setup Embedded Component

Your goal is to create a new embedded component or module for the HT16K33 project based on the hardware requirements and functionality described.

Ask for the component details if not provided:
- Component name and purpose
- Hardware interfaces required (I2C, GPIO, etc.)
- Key functionality and operations
- Performance requirements or constraints

## Requirements for the component:

### Hardware Interface
* Follow the [hardware interface guidelines](../copilot-instructions.md#hardware-specific-considerations)
* Use proper I2C communication patterns with error checking  
* Document pin assignments and hardware connections
* Include timing requirements and constraints

### Code Structure
* Follow the [C++ embedded guidelines](../instructions/cpp-embedded.instructions.md)
* Use RAII principles for resource management
* Implement proper error handling with return codes
* Minimize dynamic memory allocation

### Performance
* Apply [performance optimization guidelines](../instructions/performance.instructions.md)
* Consider memory constraints (264KB RAM on Raspberry Pi Pico)
* Optimize for real-time requirements
* Document performance characteristics

## Implementation Steps:

1. **Create header file** with class definition and public interface
2. **Implement source file** with hardware abstraction and core functionality
3. **Add initialization and cleanup** following RAII principles
4. **Include error handling** for hardware communication failures
5. **Document usage examples** and integration instructions
6. **Consider testing approach** for hardware-dependent functionality