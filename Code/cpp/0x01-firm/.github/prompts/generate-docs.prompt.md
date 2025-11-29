---
agent: 'agent'
tools: ['codebase']
description: 'Generate comprehensive documentation for embedded C++ projects'
---

# Embedded Documentation Generator

Generate comprehensive documentation for embedded C++ firmware projects, including hardware setup, API documentation, and usage guides.

## Documentation Types

### API Documentation
- **Class Documentation**: Doxygen-style comments for all public classes
- **Method Documentation**: Parameters, return values, usage examples
- **Hardware Interfaces**: Pin assignments, electrical specifications
- **Configuration Options**: Setup parameters and their effects

### User Documentation
- **Setup Guide**: Hardware connections, wiring diagrams
- **Build Instructions**: PlatformIO setup, compilation, upload
- **Usage Examples**: Common use cases and code snippets
- **Troubleshooting**: Common issues and solutions

### Developer Documentation
- **Architecture Overview**: System design and component relationships
- **Hardware Abstraction**: Interface design and implementation
- **Testing Guide**: How to run and write tests
- **Contributing Guidelines**: Development workflow and standards

## Content Generation

### Code Analysis
- Extract class and method signatures automatically
- Identify hardware dependencies and pin usage
- Generate parameter tables and configuration options
- Create interface documentation from header files

### Hardware Documentation
- **Pin Diagrams**: Visual representation of connections
- **Electrical Specifications**: Voltage levels, current limits
- **Component Lists**: Required parts and suppliers
- **Wiring Instructions**: Step-by-step connection guide

### Usage Examples
- **Basic Setup**: Minimal working example
- **Advanced Features**: Complex configuration examples
- **Integration Examples**: Using multiple components together
- **Error Handling**: Proper error detection and recovery

## Documentation Standards

### Format Requirements
- Use Markdown for most documentation
- Include syntax highlighting for code examples
- Add diagrams and images where helpful
- Use consistent heading structure and organization

### Content Guidelines
- Write for both beginners and experienced developers
- Include practical examples and use cases
- Explain embedded-specific considerations
- Keep documentation synchronized with code changes

## Generation Process
1. **Code Analysis**: Extract information from source files
2. **Content Creation**: Generate documentation sections
3. **Example Generation**: Create working code examples
4. **Format and Review**: Ensure consistency and completeness