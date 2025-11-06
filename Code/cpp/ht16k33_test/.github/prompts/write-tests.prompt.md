---
mode: 'agent'
model: Claude Sonnet 4
tools: ['codebase', 'edit/editFiles', 'runTests', 'runCommands']
description: 'Generate comprehensive tests for embedded system components'
---

# Write Embedded System Tests

Your goal is to generate comprehensive tests for embedded system components following our [testing standards](../instructions/testing.instructions.md).

Ask for the testing scope if not provided:
- Component or function to test
- Hardware dependencies (if any)
- Critical functionality to validate
- Expected edge cases or error conditions

## Test Requirements:

### Unit Tests
* Test individual functions in isolation from hardware
* Use mock objects for hardware interfaces
* Focus on business logic and data processing
* Include boundary conditions and edge cases

### Integration Tests  
* Test hardware communication (I2C, GPIO)
* Validate timing requirements and constraints
* Test error conditions and recovery scenarios
* Include timeout testing for hardware operations

### Test Organization
* Use descriptive test names that explain the scenario
* Group related tests in logical test suites
* Separate hardware-dependent from pure logic tests
* Document test setup requirements

## Testing Approach:

1. **Analyze the component** to identify testable units
2. **Create unit tests** for pure logic functions
3. **Design integration tests** for hardware interactions
4. **Implement mock objects** for hardware dependencies
5. **Add performance tests** for timing-critical functions
6. **Include error condition tests** for robustness validation
7. **Document test coverage** and any limitations

## Test Implementation:

* Use PlatformIO's native testing framework
* Follow naming conventions for test files and functions
* Include setup and teardown procedures
* Provide clear assertions and error messages
* Consider memory usage during testing