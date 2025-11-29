---
applyTo: 'test/**/*,**/*test*,**/*spec*'
description: 'Testing guidelines for embedded C++ projects'
---

# Embedded Testing Guidelines

## Testing Strategy
- Unit test core logic separately from hardware dependencies
- Use dependency injection to mock hardware interfaces
- Test edge cases and boundary conditions
- Validate timing-critical operations

## PlatformIO Testing Framework
- Use PlatformIO's built-in testing framework when possible
- Write tests in the test/ directory
- Use unity testing framework for assertions
- Organize tests by functional modules

## Hardware Abstraction for Testing
- Create mockable interfaces for hardware components
- Test business logic independently of hardware
- Use simulator environments when available
- Implement hardware-in-the-loop testing for critical paths

## Test Organization
- One test file per module or class
- Group related test cases together
- Use descriptive test names that explain the scenario
- Include both positive and negative test cases

## Embedded-Specific Testing
- Test interrupt handlers with appropriate timing
- Validate memory usage and bounds checking
- Test power management and sleep/wake cycles
- Verify real-time constraints are met

## Debugging Tests
- Use serial output for test result reporting
- Implement test fixtures for hardware setup/teardown
- Consider using LED indicators for test status
- Log test execution timing for performance validation