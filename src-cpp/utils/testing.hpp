/**
 * Unit testing framework integration
 * 
 * This header provides integration with unit testing frameworks
 * and test utilities for the pgvectorcpp library.
 */

#ifndef TESTING_H
#define TESTING_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <iostream>
#include <chrono>
#include "logging.hpp"
#include "../core/exceptions.hpp"

namespace pgvectorcpp {
namespace utils {
namespace testing {

/**
 * Test result information
 */
struct TestResult {
    std::string test_name;
    bool passed;
    std::string error_message;
    std::chrono::milliseconds duration;
};

/**
 * Base test fixture class
 */
class TestFixture {
public:
    virtual ~TestFixture() = default;
    
    /**
     * Set up test environment (called before each test)
     */
    virtual void set_up() {}
    
    /**
     * Tear down test environment (called after each test)
     */
    virtual void tear_down() {}
};

/**
 * Test case wrapper
 */
class TestCase {
public:
    using TestFunction = std::function<void()>;
    
    TestCase(const std::string& name, TestFunction function)
        : name_(name), function_(function) {}
    
    TestResult run();
    
    const std::string& name() const { return name_; }
    
private:
    std::string name_;
    TestFunction function_;
};

/**
 * Test suite container
 */
class TestSuite {
public:
    void add_test(const std::string& name, TestCase::TestFunction function);
    void add_test_with_fixture(const std::string& name, 
                              std::function<void(TestFixture&)> function,
                              std::shared_ptr<TestFixture> fixture);
    
    std::vector<TestResult> run_all();
    
private:
    std::vector<TestCase> tests_;
};

/**
 * Test runner and reporter
 */
class TestRunner {
public:
    void add_suite(const std::string& name, TestSuite suite);
    
    /**
     * Run all tests and return true if all passed
     */
    bool run_all();
    
    /**
     * Get test results
     */
    const std::vector<std::pair<std::string, TestResult>>& results() const { return results_; }
    
private:
    std::vector<std::pair<std::string, std::vector<TestResult>>> suite_results_;
    std::vector<std::pair<std::string, TestResult>> results_;
};

/**
 * Test assertion utilities
 */
class Assert {
public:
    static void is_true(bool condition, const std::string& message = "");
    static void is_false(bool condition, const std::string& message = "");
    
    template<typename T>
    static void are_equal(const T& expected, const T& actual, const std::string& message = "") {
        if (expected != actual) {
            throw std::runtime_error("Assertion failed: expected " + std::to_string(expected) + 
                                   ", got " + std::to_string(actual) + 
                                   (message.empty() ? "" : " - " + message));
        }
    }
    
    template<typename T>
    static void are_not_equal(const T& expected, const T& actual, const std::string& message = "") {
        if (expected == actual) {
            throw std::runtime_error("Assertion failed: values should not be equal" + 
                                   (message.empty() ? "" : " - " + message));
        }
    }
    
    template<typename T>
    static void is_null(const T* ptr, const std::string& message = "") {
        if (ptr != nullptr) {
            throw std::runtime_error("Assertion failed: expected null pointer" + 
                                   (message.empty() ? "" : " - " + message));
        }
    }
    
    template<typename T>
    static void is_not_null(const T* ptr, const std::string& message = "") {
        if (ptr == nullptr) {
            throw std::runtime_error("Assertion failed: expected non-null pointer" + 
                                   (message.empty() ? "" : " - " + message));
        }
    }
    
    static void throws(std::function<void()> function, const std::string& message = "");
    
    template<typename Exception>
    static void throws(std::function<void()> function, const std::string& message = "") {
        bool threw_correct = false;
        try {
            function();
        } catch (const Exception&) {
            threw_correct = true;
        } catch (...) {
            throw std::runtime_error("Assertion failed: expected specific exception type" + 
                                   (message.empty() ? "" : " - " + message));
        }
        
        if (!threw_correct) {
            throw std::runtime_error("Assertion failed: expected specific exception type" + 
                                   (message.empty() ? "" : " - " + message));
        }
    }
    
    static void does_not_throw(std::function<void()> function, const std::string& message = "");
};

/**
 * Performance test utilities
 */
class PerformanceTest {
public:
    using BenchmarkFunction = std::function<void()>;
    
    /**
     * Run a performance benchmark
     */
    static std::chrono::milliseconds benchmark(const std::string& name, 
                                             BenchmarkFunction function,
                                             int iterations = 1000);
    
    /**
     * Assert that a function completes within a time limit
     */
    static void completes_within(std::chrono::milliseconds limit,
                               BenchmarkFunction function,
                               const std::string& message = "");
};

/**
 * Memory leak detection utilities
 */
class MemoryTest {
public:
    /**
     * Check for memory leaks in a test function
     */
    static void check_for_leaks(std::function<void()> function);
    
    /**
     * Assert that a function does not allocate memory
     */
    static void does_not_allocate(std::function<void()> function);
};

/**
 * Test registration helper
 */
class TestRegistry {
public:
    static TestRegistry& instance();
    
    void register_test(const std::string& suite_name, const std::string& test_name, 
                      TestCase::TestFunction function);
    
    TestRunner create_runner();
    
private:
    TestRegistry() = default;
    std::vector<std::pair<std::string, std::pair<std::string, TestCase::TestFunction>>> tests_;
};

/**
 * Macro for registering tests
 */
#define REGISTER_TEST(suite_name, test_name) \
    static struct test_name##_registrar { \
        test_name##_registrar() { \
            pgvectorcpp::utils::testing::TestRegistry::instance().register_test( \
                #suite_name, #test_name, []() { test_name(); }); \
        } \
    } test_name##_instance;

/**
 * Macro for defining test functions
 */
#define TEST_FUNCTION(test_name) \
    void test_name(); \
    REGISTER_TEST(test_name, test_name) \
    void test_name()

} // namespace testing
} // namespace utils
} // namespace pgvectorcpp

#endif // TESTING_H
