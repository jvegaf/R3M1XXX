---
agent: 'agent'
tools: ['codebase']
description: 'Perform comprehensive code review for embedded C++ projects'
---

# Embedded Code Review Assistant

Perform thorough code review focusing on embedded systems best practices, security, and performance.

## Review Areas

### Embedded-Specific Concerns
- **Memory Management**: Check for proper memory usage, avoid dynamic allocation
- **Hardware Abstraction**: Verify proper pin usage and hardware initialization
- **Real-Time Constraints**: Validate timing-critical code sections
- **Interrupt Safety**: Check shared data access and ISR implementation
- **Power Efficiency**: Review for unnecessary power consumption

### Code Quality
- **C++ Best Practices**: Const correctness, RAII, proper data types
- **Arduino Conventions**: Appropriate use of Arduino core functions
- **Error Handling**: Proper validation and fail-safe behavior
- **Code Organization**: Separation of concerns, modularity
- **Documentation**: Clear comments and API documentation

### Security and Safety
- **Buffer Overflow Protection**: Array bounds checking
- **Input Validation**: Parameter and communication data validation
- **Resource Management**: Proper initialization and cleanup
- **Debug Information**: Avoid exposing sensitive data in debug output

## Review Process

1. **Overall Architecture Review**
   - Verify component design follows project patterns
   - Check integration with existing systems
   - Assess scalability and maintainability

2. **Line-by-Line Analysis**
   - Review critical code sections
   - Check for common embedded pitfalls
   - Validate performance-critical paths

3. **Testing and Documentation**
   - Verify adequate test coverage
   - Check documentation completeness
   - Validate example usage

## Feedback Guidelines
- Provide specific, actionable suggestions
- Prioritize critical issues (security, functionality)
- Suggest performance and maintainability improvements
- Acknowledge good practices and clean code
- Include code examples for complex suggestions