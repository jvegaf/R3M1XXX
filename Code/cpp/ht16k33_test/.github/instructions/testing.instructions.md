---
applyTo: "**/test/**,**/*test*,**/*spec*"
description: "Testing standards for embedded systems development"
---

# Testing Standards for Embedded Systems

## Testing Philosophy
- Test business logic separately from hardware interactions
- Use dependency injection to enable testing of hardware-dependent code
- Focus on critical paths and safety-critical functionality
- Write tests that document expected behavior and edge cases

## Unit Testing Approach
- Test individual functions and classes in isolation
- Mock hardware interfaces for pure logic testing
- Use deterministic test data and avoid random inputs
- Keep tests fast and repeatable

## Hardware Integration Testing
- Test I2C communication with actual hardware when possible
- Implement timeout testing for hardware operations
- Test error conditions and recovery scenarios
- Validate timing requirements and performance constraints

## Test Organization
- Separate tests by functionality (communication, logic, integration)
- Use descriptive test names that explain the scenario being tested
- Group related tests in logical test suites
- Document test setup requirements and hardware dependencies

## Embedded-Specific Testing
- Test interrupt handlers with simulated conditions
- Validate memory usage and stack depth
- Test power management and sleep mode transitions
- Verify real-time constraints and timing requirements

## Continuous Testing
- Automate tests that can run without hardware
- Set up hardware-in-the-loop testing for critical functionality
- Use static analysis tools for code quality checks
- Implement regression testing for known issues

## Test Data Management
- Use fixed test patterns for consistent results
- Document expected behaviors for different hardware states
- Test boundary conditions and error states
- Validate communication protocols with known good/bad data