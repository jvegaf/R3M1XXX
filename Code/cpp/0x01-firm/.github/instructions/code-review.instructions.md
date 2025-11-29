---
applyTo: '**/*'
description: 'Code review standards and GitHub review guidelines for embedded projects'
---

# Code Review Guidelines

## Review Focus Areas

### Functionality
- Verify code meets requirements and specifications
- Test core functionality on target hardware when possible
- Validate edge cases and error conditions
- Ensure proper initialization and cleanup

### Code Quality
- Check for consistent coding style and conventions
- Verify proper use of C++ features and Arduino APIs
- Ensure appropriate abstraction levels
- Review for code duplication and opportunities for refactoring

### Embedded-Specific Concerns
- Validate memory usage and potential overflows
- Check for proper interrupt handling
- Verify timing-critical code sections
- Ensure hardware dependencies are properly abstracted

### Security and Safety
- Review for buffer overflows and bounds checking
- Validate input sanitization and error handling
- Check for proper resource management
- Ensure fail-safe behavior in error conditions

## Review Process

### Pre-Review Checklist
- Code compiles without warnings
- Basic functionality has been tested
- Documentation is updated
- Code follows project style guidelines

### Review Standards
- Provide constructive feedback with specific suggestions
- Focus on critical issues first (security, functionality)
- Suggest improvements for performance and maintainability
- Acknowledge good practices and clean code

### GitHub Review Guidelines
- Use clear, descriptive commit messages
- Keep pull requests focused and reasonably sized
- Include test results and hardware validation notes
- Respond promptly to review feedback
- Use draft pull requests for work in progress