/**
 * Base exception hierarchy for pgvectorcpp
 * 
 * This header defines a comprehensive exception hierarchy
 * for error handling throughout the library.
 */

#ifndef VECTOR_EXCEPTIONS_H
#define VECTOR_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace pgvectorcpp {
namespace core {

/**
 * Base exception class for all pgvectorcpp exceptions
 */
class VectorException : public std::runtime_error {
public:
    explicit VectorException(const std::string& message) 
        : std::runtime_error(message) {}
    
    explicit VectorException(const char* message) 
        : std::runtime_error(message) {}
    
    virtual ~VectorException() noexcept = default;
};

/**
 * Exception thrown for invalid vector operations
 */
class VectorOperationException : public VectorException {
public:
    explicit VectorOperationException(const std::string& message)
        : VectorException("Vector operation error: " + message) {}
};

/**
 * Exception thrown for invalid vector dimensions
 */
class DimensionMismatchException : public VectorException {
public:
    explicit DimensionMismatchException(const std::string& message)
        : VectorException("Dimension mismatch: " + message) {}
};

/**
 * Exception thrown for memory allocation failures
 */
class MemoryAllocationException : public VectorException {
public:
    explicit MemoryAllocationException(const std::string& message)
        : VectorException("Memory allocation error: " + message) {}
};

/**
 * Exception thrown for I/O errors
 */
class IOException : public VectorException {
public:
    explicit IOException(const std::string& message)
        : VectorException("I/O error: " + message) {}
};

/**
 * Exception thrown for index-related errors
 */
class IndexException : public VectorException {
public:
    explicit IndexException(const std::string& message)
        : VectorException("Index error: " + message) {}
};

/**
 * Exception thrown for PostgreSQL integration errors
 */
class PostgresIntegrationException : public VectorException {
public:
    explicit PostgresIntegrationException(const std::string& message)
        : VectorException("PostgreSQL integration error: " + message) {}
};

} // namespace core
} // namespace pgvectorcpp

#endif // VECTOR_EXCEPTIONS_H
