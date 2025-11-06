---
mode: 'agent'
model: Claude Sonnet 4
tools: ['codebase', 'edit/editFiles', 'runCommands']
description: 'Generate comprehensive documentation for embedded system components'
---

# Generate Documentation

Your goal is to generate comprehensive documentation for embedded system components following our [documentation requirements](../instructions/documentation.instructions.md).

Ask for documentation scope if not provided:
- Component or module to document
- Target audience (developers, users, maintainers)
- Documentation type (API, user guide, technical specs)
- Special requirements or constraints

## Documentation Requirements:

### API Documentation
* Document all public functions with clear parameter descriptions
* Include return value specifications and error conditions
* Provide usage examples for complex interfaces
* Document preconditions and postconditions

### Hardware Documentation
* Include pin assignment tables and wiring diagrams
* Document I2C addresses and communication protocols
* Specify power requirements and operating conditions
* Include timing requirements and constraints

### User Documentation
* Provide clear setup and installation instructions
* Include configuration examples and common use cases
* Document troubleshooting procedures and known issues
* Provide safety considerations and limitations

## Documentation Process:

1. **Analyze the component** to understand its functionality
2. **Identify documentation needs** for different audiences
3. **Create structured documentation** with clear sections
4. **Include practical examples** and usage scenarios
5. **Add diagrams and visual aids** where helpful
6. **Validate accuracy** against actual implementation
7. **Ensure consistency** with project documentation standards

## Output Format:

* Use Markdown format for consistency
* Include proper headings and table of contents
* Add code examples with syntax highlighting
* Include references to related documentation
* Provide contact information for support