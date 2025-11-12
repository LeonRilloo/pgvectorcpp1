/**
 * Vector index implementations
 * 
 * This header defines the interface for vector indexing strategies
 * including HNSW and IVFFLAT implementations.
 */

#ifndef VECTOR_INDEXES_H
#define VECTOR_INDEXES_H

namespace pgvectorcpp {
namespace indexes {

// Forward declarations for index types
class HNSWIndex;
class IVFFlatIndex;
class VectorIndex;

} // namespace indexes
} // namespace pgvectorcpp

#endif // VECTOR_INDEXES_H
