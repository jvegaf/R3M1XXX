---
applyTo: "**/*.cpp,**/*.h,**/*.hpp,**/*.ino"
description: "Security best practices for embedded systems development"
---

# Security Best Practices for Embedded Systems

## Input Validation
- Validate all external inputs including serial communication and I2C data
- Check buffer boundaries to prevent overflow conditions
- Sanitize data from external sources before processing
- Implement proper error handling for malformed inputs

## Memory Safety
- Use bounds checking for array access where possible
- Avoid buffer overflows through proper size validation
- Clear sensitive data from memory after use
- Use stack canaries and memory protection where available

## Communication Security
- Validate I2C communication protocols and data integrity
- Implement timeout mechanisms for external communications
- Consider encryption for sensitive data transmission
- Use authentication mechanisms for critical commands

## Access Control
- Implement proper privilege separation for different functions
- Protect critical system functions from unauthorized access
- Use hardware security features when available
- Document security boundaries and assumptions

## Error Handling
- Fail securely when errors occur
- Avoid information leakage through error messages
- Implement proper logging without exposing sensitive information
- Use secure defaults for configuration parameters

## Code Quality
- Use static analysis tools to identify potential security issues
- Follow secure coding practices and guidelines
- Review code for common vulnerability patterns
- Keep dependencies and libraries updated

## Hardware Security
- Protect against physical tampering when possible
- Use hardware random number generators for cryptographic operations
- Implement secure boot processes where applicable
- Consider side-channel attack protection for sensitive operations