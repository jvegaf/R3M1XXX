---
mode: 'agent' 
model: Claude Sonnet 4
tools: ['codebase', 'edit/editFiles', 'usages', 'runTests']
description: 'Refactor embedded C++ code while maintaining functionality and improving quality'
---

# Refactor Embedded Code

Your goal is to refactor existing embedded C++ code to improve maintainability, performance, and code quality while preserving functionality.

Ask for refactoring scope if not provided:
- Code areas to refactor
- Specific quality issues to address
- Performance goals or constraints
- Compatibility requirements

## Refactoring Approach:

### Safety First
* Preserve existing functionality and behavior
* Maintain hardware interface compatibility  
* Keep performance characteristics within acceptable bounds
* Ensure thread safety and interrupt compatibility

### Code Quality Improvements
* Apply [C++ embedded guidelines](../instructions/cpp-embedded.instructions.md)
* Improve error handling and resource management
* Enhance code readability and maintainability
* Optimize memory usage and allocation patterns

### Performance Optimization
* Follow [performance guidelines](../instructions/performance.instructions.md)
* Reduce memory footprint where possible
* Optimize critical execution paths
* Improve real-time behavior and determinism

## Refactoring Steps:

1. **Analyze existing code** and identify improvement opportunities
2. **Plan refactoring approach** with incremental changes
3. **Preserve existing tests** and add new ones if needed
4. **Apply refactoring patterns** appropriate for embedded systems
5. **Validate functionality** through testing
6. **Measure performance impact** of changes
7. **Document refactoring decisions** and trade-offs

## Focus Areas:

* Extract hardware abstraction layers
* Improve error handling consistency
* Optimize data structures and algorithms
* Enhance code modularity and reusability
* Reduce coupling between components
* Improve resource management patterns