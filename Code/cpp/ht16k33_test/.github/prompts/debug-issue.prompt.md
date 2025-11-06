---
mode: 'agent'
model: Claude Sonnet 4  
tools: ['codebase', 'problems', 'runCommands', 'runTests', 'search', 'usages']
description: 'Debug embedded system issues and hardware communication problems'
---

# Debug Embedded Issues

Your goal is to systematically debug embedded system issues, particularly focusing on hardware communication and real-time behavior problems.

Ask for debug context if not provided:
- Description of the issue or unexpected behavior
- Hardware setup and configuration
- Steps to reproduce the problem
- Error messages or symptoms observed

## Debugging Approach:

### Problem Assessment
* Gather detailed error information and symptoms
* Understand the hardware setup and connections
* Identify the expected vs actual behavior
* Review recent changes that might have introduced the issue

### Hardware-Specific Debugging
* Check I2C communication integrity and addressing
* Validate power supply and voltage levels
* Verify timing requirements and signal integrity
* Test hardware connections and pin assignments

### Software Analysis
* Trace code execution paths leading to the issue
* Examine variable states and data flows
* Check for common embedded issues (race conditions, memory corruption)
* Validate interrupt handling and critical sections

## Debugging Steps:

1. **Reproduce the issue** consistently
2. **Analyze hardware setup** and connections
3. **Check communication protocols** (I2C, serial, etc.)
4. **Trace software execution** and data flow
5. **Identify root cause** through systematic elimination
6. **Implement targeted fix** with minimal changes
7. **Validate resolution** through comprehensive testing

## Common Issues to Check:

* I2C addressing and communication errors
* Timing violations and race conditions
* Memory corruption and stack overflow
* Interrupt conflicts and priority issues
* Power supply and voltage problems
* Hardware connection and wiring issues

## Debugging Tools:

* Serial debugging and logging
* Hardware analysis tools (oscilloscope, logic analyzer)
* Memory and stack analysis
* Static code analysis for potential issues