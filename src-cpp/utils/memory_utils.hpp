/**
 * Memory management utilities and smart pointer wrappers
 * 
 * This header provides memory management utilities including
 * custom smart pointer wrappers for safe memory handling.
 */

#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include <memory>
#include <vector>
#include <cstddef>
#include "vector_utils.h"
#include "../core/exceptions.hpp"

namespace pgvectorcpp {
namespace utils {

/**
 * Custom deleter for arrays allocated with aligned_alloc
 */
struct AlignedDeleter {
    void operator()(void* ptr) const {
        if (ptr) {
            free(ptr);
        }
    }
};

/**
 * Unique pointer for aligned memory
 */
template<typename T>
using aligned_unique_ptr = std::unique_ptr<T[], AlignedDeleter>;

/**
 * Memory alignment utilities
 */
class MemoryAlignment {
public:
    static constexpr size_t VECTOR_ALIGNMENT = 32; // AVX2 alignment
    
    /**
     * Allocate aligned memory for vectors
     */
    template<typename T>
    static aligned_unique_ptr<T> allocate_aligned(size_t count) {
        void* ptr = aligned_alloc(VECTOR_ALIGNMENT, count * sizeof(T));
        if (!ptr) {
            throw core::MemoryAllocationException("Failed to allocate aligned memory");
        }
        return aligned_unique_ptr<T>(static_cast<T*>(ptr));
    }
    
    /**
     * Check if pointer is aligned
     */
    template<typename T>
    static bool is_aligned(const T* ptr) {
        return reinterpret_cast<uintptr_t>(ptr) % VECTOR_ALIGNMENT == 0;
    }
};

/**
 * Memory pool for efficient vector allocations
 */
class MemoryPool {
public:
    explicit MemoryPool(size_t block_size = 1024 * 1024) // 1MB blocks
        : block_size_(block_size), current_offset_(0) {}
    
    /**
     * Allocate memory from the pool
     */
    void* allocate(size_t size) {
        size = align_size(size);
        
        if (current_offset_ + size > block_size_) {
            allocate_new_block();
        }
        
        void* ptr = static_cast<char*>(current_block_.get()) + current_offset_;
        current_offset_ += size;
        return ptr;
    }
    
    /**
     * Reset the pool (frees all memory)
     */
    void reset() {
        blocks_.clear();
        current_block_.reset();
        current_offset_ = 0;
    }
    
private:
    size_t block_size_;
    size_t current_offset_;
    std::vector<std::unique_ptr<char[]>> blocks_;
    std::unique_ptr<char[]> current_block_;
    
    static constexpr size_t align_size(size_t size) {
        return (size + alignof(std::max_align_t) - 1) & ~(alignof(std::max_align_t) - 1);
    }
    
    void allocate_new_block() {
        current_block_ = std::make_unique<char[]>(block_size_);
        blocks_.push_back(std::move(current_block_));
        current_offset_ = 0;
    }
};

/**
 * RAII wrapper for memory-mapped files
 */
class MemoryMappedFile {
public:
    MemoryMappedFile(const std::string& path, size_t size);
    ~MemoryMappedFile();
    
    // Delete copy operations
    MemoryMappedFile(const MemoryMappedFile&) = delete;
    MemoryMappedFile& operator=(const MemoryMappedFile&) = delete;
    
    // Enable move operations
    MemoryMappedFile(MemoryMappedFile&& other) noexcept;
    MemoryMappedFile& operator=(MemoryMappedFile&& other) noexcept;
    
    /**
     * Get pointer to mapped memory
     */
    void* data() { return data_; }
    const void* data() const { return data_; }
    
    /**
     * Get size of mapped region
     */
    size_t size() const { return size_; }
    
private:
    void* data_;
    size_t size_;
    int fd_;
};

} // namespace utils
} // namespace pgvectorcpp

#endif // MEMORY_UTILS_H
