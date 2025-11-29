---
agent: 'agent'
tools: ['codebase']
description: 'Generate unit tests for embedded C++ components'
---

# Embedded Test Generator

Create comprehensive unit tests for embedded C++ components using PlatformIO testing framework.

## Test Strategy
Generate tests that:
- Test core functionality independently of hardware
- Mock hardware dependencies when possible
- Validate edge cases and error conditions
- Check memory bounds and overflow conditions
- Verify timing-critical operations

## Test Structure
Create test files with:

### Test Organization
- One test file per component in test/ directory
- Use descriptive test function names
- Group related tests using test suites
- Include setup and teardown functions

### Test Categories
- **Initialization tests**: Verify proper component setup
- **Functional tests**: Test core component behavior
- **Edge case tests**: Validate boundary conditions
- **Error handling tests**: Test failure scenarios
- **Integration tests**: Test component interactions

## Embedded Testing Patterns

### Hardware Mocking
- Create mock interfaces for hardware dependencies
- Use dependency injection for testability
- Simulate hardware responses and timing
- Test without actual hardware when possible

### Test Utilities
- Use Unity testing framework assertions
- Include timing measurements for performance tests
- Add memory usage validation
- Implement test fixtures for hardware setup

### Serial Output Testing
- Capture and validate serial debug output
- Test error message generation
- Verify logging and diagnostic information

## Code Generation
- Follow Unity testing conventions
- Include necessary headers and test runner setup
- Add meaningful assertion messages
- Document test scenarios and expected outcomes
- Consider real-time constraints in test design