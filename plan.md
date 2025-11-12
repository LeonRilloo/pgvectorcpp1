# C to C++ Conversion Plan for pgvector

## Overview
This plan outlines the systematic conversion of the C codebase in `src/` to modern C++ in `src-cpp/`. The conversion will focus on maintaining functionality while implementing modern C++ best practices including RAII, smart pointers, classes, and exception safety.

## Phase 1: Project Setup and Infrastructure

### Step 1.1: Create Directory Structure
- Create `src-cpp/` directory
- Create subdirectories for organized code structure:
  - `src-cpp/core/` - Core vector data structures
  - `src-cpp/indexes/` - Index implementations (HNSW, IVFFLAT)
  - `src-cpp/utils/` - Utility functions and helpers
  - `src-cpp/io/` - Input/output operations
  - `src-cpp/postgres/` - PostgreSQL integration layer

### Step 1.2: Set up Modern C++ Build System
- Create CMakeLists.txt for C++ build configuration
- Set C++20 standard as minimum requirement
- Configure compiler flags for optimization and warnings
- Set up dependency management for PostgreSQL headers

### Step 1.3: Create Core C++ Infrastructure
- Implement base exception hierarchy
- Create memory management utilities (smart pointer wrappers)
- Set up logging framework
- Create unit testing framework integration

## Phase 2: Core Data Structures Conversion

### Step 2.1: Convert Vector Structure
**Source Files:** `src/vector.h`, `src/vector.c`
**Target Files:** `src-cpp/core/vector.hpp`, `src-cpp/core/vector.cpp`

**Conversion Strategy:**
- Convert C struct to C++ class with proper encapsulation
- Implement constructors, destructor, copy/move semantics
- Use std::vector<float> for flexible array member
- Add operator overloading for vector operations
- Implement RAII for memory management
- Add template specializations for different numeric types

**Key Considerations:**
- Maintain binary compatibility with PostgreSQL storage format
- Preserve PG_DETOAST_DATUM macro functionality
- Handle FLEXIBLE_ARRAY_MEMBER equivalent in C++

### Step 2.2: Convert Half Vector
**Source Files:** `src/halfvec.h`, `src/halfvec.c`
**Target Files:** `src-cpp/core/half_vector.hpp`, `src-cpp/core/half_vector.cpp`

**Conversion Strategy:**
- Create template class for half-precision vectors
- Implement proper type safety for half-float operations
- Use modern C++ type traits for numeric operations

### Step 2.3: Convert Sparse Vector
**Source Files:** `src/sparsevec.h`, `src/sparsevec.c`
**Target Files:** `src-cpp/core/sparse_vector.hpp`, `src-cpp/core/sparse_vector.cpp`

**Conversion Strategy:**
- Use std::unordered_map or custom sparse representation
- Implement iterators for sparse data structures
- Add template parameters for index and value types

## Phase 3: Utility Functions Conversion

### Step 3.1: Convert Bit Utilities
**Source Files:** `src/bitutils.h`, `src/bitutils.c`, `src/bitvec.h`, `src/bitvec.c`
**Target Files:** `src-cpp/utils/bit_utils.hpp`, `src-cpp/utils/bit_utils.cpp`

**Conversion Strategy:**
- Create template-based bit manipulation utilities
- Use constexpr for compile-time bit operations
- Implement type-safe bit vector class
- Add SIMD optimizations where applicable

### Step 3.2: Convert Half Utilities
**Source Files:** `src/halfutils.h`, `src/halfutils.c`
**Target Files:** `src-cpp/utils/half_utils.hpp`, `src-cpp/utils/half_utils.cpp`

**Conversion Strategy:**
- Create modern C++ wrapper for half-precision floats
- Use std::uint16_t for storage with proper type safety
- Implement IEEE 754 compliant operations

## Phase 4: Index Implementation Conversion

### Step 4.1: Convert HNSW Index
**Source Files:** `src/hnsw.h`, `src/hnsw.c`, `src/hnswbuild.c`, `src/hnswinsert.c`, `src/hnswscan.c`, `src/hnswutils.c`, `src/hnswvacuum.c`
**Target Files:** `src-cpp/indexes/hnsw/`

**Conversion Strategy:**
- Create HNSW class with proper encapsulation
- Use smart pointers for graph node management
- Implement RAII for resource management
- Create template-based distance functions
- Use modern C++ containers for graph structures
- Implement exception-safe construction and destruction

**Key Components:**
- `hnsw.hpp` - Main HNSW class interface
- `hnsw_graph.hpp` - Graph structure management
- `hnsw_builder.hpp` - Index construction
- `hnsw_searcher.hpp` - Search operations
- `hnsw_vacuum.hpp` - Maintenance operations

### Step 4.2: Convert IVFFLAT Index
**Source Files:** `src/ivfflat.h`, `src/ivfflat.c`, `src/ivfbuild.c`, `src/ivfinsert.c`, `src/ivfscan.c`, `src/ivfutils.c`, `src/ivfvacuum.c`, `src/ivfkmeans.c`
**Target Files:** `src-cpp/indexes/ivfflat/`

**Conversion Strategy:**
- Create IVFFLAT class hierarchy
- Implement k-means clustering as template function
- Use modern C++ algorithms for clustering operations
- Create RAII-based resource management for inverted lists

**Key Components:**
- `ivfflat.hpp` - Main IVFFLAT class interface
- `ivfflat_clustering.hpp` - K-means clustering implementation
- `ivfflat_builder.hpp` - Index construction
- `ivfflat_searcher.hpp` - Search operations
- `ivfflat_vacuum.hpp` - Maintenance operations

## Phase 5: PostgreSQL Integration Layer

### Step 5.1: Create PostgreSQL Wrapper Layer
**Target Files:** `src-cpp/postgres/`

**Conversion Strategy:**
- Create C++ wrappers for PostgreSQL C API calls
- Implement RAII for PostgreSQL memory contexts
- Create exception translation from PostgreSQL errors to C++ exceptions
- Design interface layer for type conversion

### Step 5.2: Convert Extension Entry Points
**Source Files:** Entry point functions in C files
**Target Files:** `src-cpp/postgres/extension.hpp`

**Conversion Strategy:**
- Create extern "C" wrapper functions for PostgreSQL compatibility
- Implement proper exception handling at C/C++ boundary
- Maintain ABI compatibility with existing extension interface

## Phase 6: Testing and Validation

### Step 6.1: Create Unit Tests
- Write comprehensive unit tests for each converted component
- Use modern C++ testing framework (Google Test or Catch2)
- Ensure test coverage matches original C code

### Step 6.2: Integration Testing
- Test PostgreSQL extension loading and functionality
- Validate performance against original C implementation
- Test memory management and resource cleanup

### Step 6.3: Performance Benchmarking
- Create performance benchmarks comparing C and C++ versions
- Profile memory usage and identify optimization opportunities
- Validate that modern C++ features don't degrade performance

## Phase 7: Documentation and Code Quality

### Step 7.1: Documentation
- Document all public APIs with modern C++ documentation standards
- Create migration guide for developers
- Document memory management patterns and ownership semantics

### Step 7.2: Code Quality Tools
- Set up static analysis tools (clang-tidy, cppcheck)
- Configure automatic formatting (clang-format)
- Implement continuous integration for C++ code

### Step 7.3: Modern C++ Best Practices
- Use const-correctness throughout
- Implement Rule of Five where applicable
- Use noexcept specifications appropriately
- Apply modern C++ idioms and patterns

## Conversion Guidelines and Best Practices

### Memory Management
- Replace manual memory allocation with smart pointers
- Use RAII for all resource management
- Implement custom deleters for PostgreSQL-specific resources
- Avoid raw pointers except at PostgreSQL interface boundary

### Error Handling
- Convert error codes to C++ exceptions
- Create custom exception hierarchy for different error types
- Ensure exception safety guarantees
- Translate PostgreSQL errors to C++ exceptions

### Performance Considerations
- Use move semantics to avoid unnecessary copies
- Implement template specializations for performance-critical paths
- Utilize modern C++ algorithms and containers
- Consider SIMD optimizations where applicable

### Compatibility Requirements
- Maintain binary compatibility with existing PostgreSQL data files
- Preserve ABI compatibility at C interface boundary
- Ensure existing SQL interfaces continue to work
- Support existing extension upgrade paths

## Risk Mitigation

### Technical Risks
1. **Performance Degradation**: Benchmark continuously during conversion
2. **Memory Leaks**: Use sanitizers and valgrind for validation
3. **Binary Incompatibility**: Test with existing databases
4. **PostgreSQL Version Compatibility**: Test across multiple PostgreSQL versions

### Mitigation Strategies
- Incremental conversion with rollback capability
- Comprehensive testing at each phase
- Performance monitoring and optimization
- Code review process for all conversions

## Timeline and Milestones

### Milestone 1: Infrastructure (Week 1-2)
- Directory structure and build system
- Core infrastructure and utilities

### Milestone 2: Core Data Structures (Week 3-4)
- Vector, HalfVector, SparseVector conversion
- Unit tests for core structures

### Milestone 3: Utilities (Week 5)
- Bit utilities and half utilities conversion
- Performance validation

### Milestone 4: Index Implementations (Week 6-8)
- HNSW index conversion
- IVFFLAT index conversion
- Integration testing

### Milestone 5: PostgreSQL Integration (Week 9)
- Extension interface conversion
- End-to-end testing

### Milestone 6: Final Validation (Week 10)
- Performance benchmarking
- Code quality review
- Documentation completion

## Success Criteria

1. All C code successfully converted to modern C++
2. No performance regression compared to original implementation
3. Zero memory leaks and resource management issues
4. Full compatibility with existing PostgreSQL installations
5. Comprehensive test coverage matching original code
6. Modern C++ best practices followed throughout
7. Complete documentation and developer guides

This plan provides a systematic approach to converting the pgvector extension from C to modern C++ while maintaining functionality, performance, and compatibility.
