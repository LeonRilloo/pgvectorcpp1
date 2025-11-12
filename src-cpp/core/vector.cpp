#include "vector.hpp"
#include <stdexcept>
#include <cstring>

Vector::Vector(int16 dimensions) : vl_len_(0), dim(dimensions), unused(0) {
    validate_dimension(dimensions);
    data.resize(dimensions);
}

Vector::Vector(const Vector& other) 
    : vl_len_(other.vl_len_), 
      dim(other.dim), 
      unused(other.unused),
      data(other.data) {
}

Vector::Vector(Vector&& other) noexcept
    : vl_len_(other.vl_len_),
      dim(other.dim),
      unused(other.unused),
      data(std::move(other.data)) {
    other.dim = 0;
    other.vl_len_ = 0;
}

Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        vl_len_ = other.vl_len_;
        dim = other.dim;
        unused = other.unused;
        data = other.data;
    }
    return *this;
}

Vector& Vector::operator=(Vector&& other) noexcept {
    if (this != &other) {
        vl_len_ = other.vl_len_;
        dim = other.dim;
        unused = other.unused;
        data = std::move(other.data);
        other.dim = 0;
        other.vl_len_ = 0;
    }
    return *this;
}

Vector Vector::operator+(const Vector& other) const {
    check_compatibility(other);
    Vector result(dim);
    for (size_t i = 0; i < data.size(); ++i) {
        result.data[i] = data[i] + other.data[i];
    }
    return result;
}

Vector Vector::operator-(const Vector& other) const {
    check_compatibility(other);
    Vector result(dim);
    for (size_t i = 0; i < data.size(); ++i) {
        result.data[i] = data[i] - other.data[i];
    }
    return result;
}

Vector Vector::operator*(const Vector& other) const {
    check_compatibility(other);
    Vector result(dim);
    for (size_t i = 0; i < data.size(); ++i) {
        result.data[i] = data[i] * other.data[i];
    }
    return result;
}

Vector Vector::operator*(float scalar) const {
    Vector result(dim);
    for (size_t i = 0; i < data.size(); ++i) {
        result.data[i] = data[i] * scalar;
    }
    return result;
}

bool Vector::operator==(const Vector& other) const {
    if (dim != other.dim) return false;
    return std::equal(data.begin(), data.end(), other.data.begin());
}

bool Vector::operator!=(const Vector& other) const {
    return !(*this == other);
}

bool Vector::operator<(const Vector& other) const {
    int16 min_dim = std::min(dim, other.dim);
    for (int i = 0; i < min_dim; ++i) {
        if (data[i] < other.data[i]) return true;
        if (data[i] > other.data[i]) return false;
    }
    return dim < other.dim;
}

bool Vector::operator<=(const Vector& other) const {
    return (*this < other) || (*this == other);
}

bool Vector::operator>(const Vector& other) const {
    return !(*this <= other);
}

bool Vector::operator>=(const Vector& other) const {
    return !(*this < other);
}

float Vector::l2_norm() const {
    float sum = 0.0f;
    for (float val : data) {
        sum += val * val;
    }
    return std::sqrt(sum);
}

float Vector::l1_norm() const {
    float sum = 0.0f;
    for (float val : data) {
        sum += std::abs(val);
    }
    return sum;
}

float Vector::dot_product(const Vector& other) const {
    check_compatibility(other);
    float sum = 0.0f;
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i] * other.data[i];
    }
    return sum;
}

float Vector::cosine_similarity(const Vector& other) const {
    float dot = dot_product(other);
    float norm_a = l2_norm();
    float norm_b = other.l2_norm();
    
    if (norm_a == 0.0f || norm_b == 0.0f) {
        return 0.0f; // Return 0 for zero vectors
    }
    
    return dot / (norm_a * norm_b);
}

Vector* Vector::fromDatum(Datum datum) {
    // For testing purposes, just create a simple vector
    Vector* vec = new Vector(3);
    vec->data[0] = 1.0f;
    vec->data[1] = 2.0f;
    vec->data[2] = 3.0f;
    return vec;
}

Datum Vector::toDatum() const {
    // For testing purposes, just return a simple pointer
    return (Datum)this;
}

void Vector::serialize(char* buffer) const {
    // Write header
    memcpy(buffer, &vl_len_, sizeof(int32));
    buffer += sizeof(int32);
    memcpy(buffer, &dim, sizeof(int16));
    buffer += sizeof(int16);
    memcpy(buffer, &unused, sizeof(int16));
    buffer += sizeof(int16);
    
    // Write data
    memcpy(buffer, data.data(), dim * sizeof(float));
}

Vector Vector::deserialize(const char* buffer) {
    int32 vl_len;
    int16 dimensions;
    int16 unused_val;
    
    // Read header
    memcpy(&vl_len, buffer, sizeof(int32));
    buffer += sizeof(int32);
    memcpy(&dimensions, buffer, sizeof(int16));
    buffer += sizeof(int16);
    memcpy(&unused_val, buffer, sizeof(int16));
    buffer += sizeof(int16);
    
    // Create vector and read data
    Vector vec(dimensions);
    vec.vl_len_ = vl_len;
    vec.unused = unused_val;
    
    memcpy(vec.data.data(), buffer, dimensions * sizeof(float));
    
    return vec;
}

void Vector::validate_dimension(int16 d) const {
    if (d < 1) {
        throw std::runtime_error("vector must have at least 1 dimension");
    }
    if (d > VECTOR_MAX_DIM) {
        throw std::runtime_error("vector cannot have more than " + std::to_string(VECTOR_MAX_DIM) + " dimensions");
    }
}

void Vector::validate_index(size_t index) const {
    if (index >= static_cast<size_t>(dim)) {
        throw std::runtime_error("vector index out of range");
    }
}

void Vector::check_compatibility(const Vector& other) const {
    if (dim != other.dim) {
        throw std::runtime_error("different vector dimensions " + std::to_string(dim) + " and " + std::to_string(other.dim));
    }
}
