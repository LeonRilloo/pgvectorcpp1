/**
 * PostgreSQL integration layer
 * 
 * This header provides the interface between the C++ vector
 * operations and PostgreSQL database functionality.
 */

#ifndef PG_INTEGRATION_H
#define PG_INTEGRATION_H

namespace pgvectorcpp {
namespace postgres {

// PostgreSQL integration declarations
class PGConnection;
class PGTypeConverter;
class PGFunctionBinding;

} // namespace postgres
} // namespace pgvectorcpp

#endif // PG_INTEGRATION_H
