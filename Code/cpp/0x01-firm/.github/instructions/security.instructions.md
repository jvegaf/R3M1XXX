---
applyTo: '**/*'
description: 'Security best practices for embedded firmware development'
---

# Embedded Security Guidelines

## Code Security
- Validate all input parameters and bounds
- Avoid buffer overflows with proper array bounds checking
- Use const correctness to prevent unintended modifications
- Implement proper error handling and fail-safe modes

## Memory Protection
- Initialize all variables before use
- Use stack canaries when supported
- Avoid exposing sensitive data in debug output
- Clear sensitive data from memory after use

## Communication Security
- Validate serial communication inputs
- Implement checksums for data integrity
- Use secure protocols when communicating with external devices
- Avoid transmitting sensitive information in plain text

## Hardware Security
- Protect against hardware tampering when possible
- Implement watchdog timers for system reliability
- Use secure boot processes when available
- Protect debug interfaces in production builds

## Access Control
- Implement privilege separation when applicable
- Use authentication for administrative functions
- Limit access to critical system functions
- Log security-relevant events when storage permits

## Secure Development Practices
- Review code for security vulnerabilities
- Use static analysis tools when available
- Keep libraries and dependencies updated
- Follow secure coding standards for embedded systems