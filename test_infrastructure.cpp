#include "src-cpp/core/exceptions.hpp"
#include "src-cpp/utils/memory_utils.hpp"
#include "src-cpp/utils/logging.hpp"
#include "src-cpp/utils/testing.hpp"
#include <thread>
#include <iostream>

using namespace pgvectorcpp;

void test_exceptions() {
    std::cout << "Testing exception hierarchy..." << std::endl;
    
    try {
        throw core::VectorOperationException("Test operation error");
    } catch (const core::VectorException& e) {
        std::cout << "Caught VectorException: " << e.what() << std::endl;
    }
    
    try {
        throw core::DimensionMismatchException("Test dimension error");
    } catch (const core::VectorException& e) {
        std::cout << "Caught VectorException: " << e.what() << std::endl;
    }
}

void test_memory_utils() {
    std::cout << "Testing memory utilities..." << std::endl;
    
    // Test aligned allocation
    auto ptr = utils::MemoryAlignment::allocate_aligned<float>(100);
    std::cout << "Allocated aligned memory: " << ptr.get() << std::endl;
    std::cout << "Is aligned: " << utils::MemoryAlignment::is_aligned(ptr.get()) << std::endl;
    
    // Test memory pool
    utils::MemoryPool pool;
    void* p1 = pool.allocate(1024);
    void* p2 = pool.allocate(2048);
    std::cout << "Allocated from memory pool: " << p1 << ", " << p2 << std::endl;
    pool.reset();
}

void test_logging() {
    std::cout << "Testing logging framework..." << std::endl;
    
    // Initialize logging
    auto logger = std::make_shared<utils::CompositeLogger>();
    logger->add_logger(std::make_shared<utils::ConsoleLogger>());
    logger->add_logger(std::make_shared<utils::FileLogger>("test.log"));
    utils::Log::init(logger);
    
    // Test different log levels
    utils::Log::debug("Debug message");
    utils::Log::info("Info message");
    utils::Log::warning("Warning message");
    utils::Log::error("Error message");
    
    // Test log timer
    {
        utils::LogTimer timer("Test operation");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void test_testing_framework() {
    std::cout << "Testing testing framework..." << std::endl;
    
    // Create a test suite
    utils::testing::TestSuite suite;
    
    // Add a simple test
    suite.add_test("Simple test", []() {
        utils::testing::Assert::is_true(1 + 1 == 2, "Math is broken");
        utils::testing::Assert::are_equal(42, 42, "The answer is wrong");
    });
    
    // Add a test that throws
    suite.add_test("Exception test", []() {
        utils::testing::Assert::throws<core::VectorException>([]() {
            throw core::VectorException("Test exception");
        }, "Should throw VectorException");
    });
    
    // Run the tests
    auto results = suite.run_all();
    for (const auto& result : results) {
        std::cout << result.test_name << ": " << (result.passed ? "PASS" : "FAIL") << std::endl;
        if (!result.passed) {
            std::cout << "  Error: " << result.error_message << std::endl;
        }
    }
}

int main() {
    try {
        test_exceptions();
        test_memory_utils();
        test_logging();
        test_testing_framework();
        
        std::cout << "All tests completed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
