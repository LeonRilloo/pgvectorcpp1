# C++ Source Directory Structure

This directory contains the C++ implementation of the pgvector functionality,
organized into logical subdirectories for maintainability and clarity.

## Directory Structure

### `core/`
Core vector data structures and fundamental operations.
This directory contains the basic building blocks for vector operations,
including vector representations, basic arithmetic operations, and memory management.

### `indexes/`
Index implementations for efficient vector search.
Contains implementations of various vector indexing algorithms including:
- HNSW (Hierarchical Navigable Small World) graphs
- IVFFLAT (Inverted File with Flat compression)
- Other vector indexing strategies

### `utils/`
Utility functions and helpers.
Provides common functionality used across the codebase including:
- Mathematical utilities
- Memory management helpers
- Error handling utilities
- Common data transformations

### `io/`
Input/output operations.
Handles reading and writing vector data in various formats:
- Binary I/O operations
- Text format parsing
- Serialization/deserialization
- File format converters

### `postgres/`
PostgreSQL integration layer.
Provides the bridge between the C++ vector operations and PostgreSQL:
- PostgreSQL extension interface
- SQL function bindings
- Type conversion utilities
- Database connection management
