/**
 * Input/output operations for vector data
 * 
 * This header defines interfaces for reading and writing
 * vector data in various formats.
 */

#ifndef VECTOR_IO_H
#define VECTOR_IO_H

namespace pgvectorcpp {
namespace io {

// I/O interface declarations
class VectorReader;
class VectorWriter;
class BinaryFormat;
class TextFormat;

} // namespace io
} // namespace pgvectorcpp

#endif // VECTOR_IO_H
