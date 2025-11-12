#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <stdexcept>

// PostgreSQL-compatible constants
#define VECTOR_MAX_DIM 16000

// Simplified Datum type for testing
typedef void* Datum;

// Error handling macros
#define ereport(elevel, ...) \
    do { \
        printf("ERROR: "); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        throw std::runtime_error("PostgreSQL error"); \
    } while(0)

#define errcode(code) code
#define errmsg(fmt, ...) fmt, ##__VA_ARGS__

#define ERRLEVEL_ERROR 1
#define ERRCODE_DATA_EXCEPTION "22000"
#define ERRCODE_PROGRAM_LIMIT_EXCEEDED "54000"

// Simplified PG_DETOAST_DATUM for testing
#define PG_DETOAST_DATUM(x) (x)

// PostgreSQL type definitions
using int32 = int32_t;
using int16 = int16_t;

class Vector {
private:
    int32 vl_len_;  // varlena header (do not touch directly!)
    int16 dim;      // number of dimensions
    int16 unused;   // reserved for future use, always zero
    std::vector<float> data;

public:
    // Constructors
    Vector() : vl_len_(0), dim(0), unused(0) {}
    explicit Vector(int16 dimensions);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    
    // Destructor
    ~Vector() = default;

    // Assignment operators
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    // Element access
    float& operator[](size_t index) { return data[index]; }
    const float& operator[](size_t index) const { return data[index]; }
    
    // Vector operations
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(const Vector& other) const;
    Vector operator*(float scalar) const;
    
    // Comparison operators
    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;
    bool operator<(const Vector& other) const;
    bool operator<=(const Vector& other) const;
    bool operator>(const Vector& other) const;
    bool operator>=(const Vector& other) const;

    // Utility functions
    int16 getDim() const { return dim; }
    float getValue(size_t index) const { return data[index]; }
    void setValue(size_t index, float value) { data[index] = value; }
    float l2_norm() const;
    float l1_norm() const;
    float dot_product(const Vector& other) const;
    float cosine_similarity(const Vector& other) const;
    
    // PostgreSQL compatibility
    static Vector* fromDatum(Datum datum);
    Datum toDatum() const;
    
    // Binary I/O
    void serialize(char* buffer) const;
    static Vector deserialize(const char* buffer);

private:
    void validate_dimension(int16 d) const;
    void validate_index(size_t index) const;
    void check_compatibility(const Vector& other) const;
};

// Non-member overloads for scalar multiplication
inline Vector operator*(float scalar, const Vector& vec) {
    return vec * scalar;
}

#endif // VECTOR_HPP
