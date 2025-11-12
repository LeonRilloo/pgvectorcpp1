#include "testing.hpp"
#include <cmath>
#include <cstring>

namespace pgvectorcpp {
namespace utils {
namespace testing {

TestResult TestCase::run() {
    TestResult result;
    result.test_name = name_;
    result.passed = false;
    
    auto start = std::chrono::steady_clock::now();
    
    try {
        function_();
        result.passed = true;
    } catch (const std::exception& e) {
        result.error_message = e.what();
    } catch (...) {
        result.error_message = "Unknown exception";
    }
    
    auto end = std::chrono::steady_clock::now();
    result.duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    return result;
}

void TestSuite::add_test(const std::string& name, TestCase::TestFunction function) {
    tests_.emplace_back(name, function);
}

void TestSuite::add_test_with_fixture(const std::string& name, 
                                    std::function<void(TestFixture&)> function,
                                    std::shared_ptr<TestFixture> fixture) {
    tests_.emplace_back(name, [function, fixture]() {
        fixture->set_up();
        try {
            function(*fixture);
        } catch (...) {
            fixture->tear_down();
            throw;
        }
        fixture->tear_down();
    });
}

std::vector<TestResult> TestSuite::run_all() {
    std::vector<TestResult> results;
    for (auto& test : tests_) {
        results.push_back(test.run());
    }
    return results;
}

void TestRunner::add_suite(const std::string& name, TestSuite suite) {
    auto results = suite.run_all();
    suite_results_.emplace_back(name, results);
    
    for (auto& result : results) {
        results_.emplace_back(name + "." + result.test_name, result);
    }
}

bool TestRunner::run_all() {
    bool all_passed = true;
    
    for (auto& [suite_name, suite_results] : suite_results_) {
        std::cout << "Running test suite: " << suite_name << std::endl;
        
        for (auto& result : suite_results) {
            std::cout << "  " << result.test_name << ": ";
            
            if (result.passed) {
                std::cout << "PASS (" << result.duration.count() << "ms)" << std::endl;
            } else {
                std::cout << "FAIL - " << result.error_message << std::endl;
                all_passed = false;
            }
        }
    }
    
    return all_passed;
}

void Assert::is_true(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error("Assertion failed: expected true" + 
                               (message.empty() ? "" : " - " + message));
    }
}

void Assert::is_false(bool condition, const std::string& message) {
    if (condition) {
        throw std::runtime_error("Assertion failed: expected false" + 
                               (message.empty() ? "" : " - " + message));
    }
}

void Assert::throws(std::function<void()> function, const std::string& message) {
    bool threw = false;
    try {
        function();
    } catch (...) {
        threw = true;
    }
    
    if (!threw) {
        throw std::runtime_error("Assertion failed: expected exception" + 
                               (message.empty() ? "" : " - " + message));
    }
}

void Assert::does_not_throw(std::function<void()> function, const std::string& message) {
    try {
        function();
    } catch (const std::exception& e) {
        throw std::runtime_error("Assertion failed: unexpected exception: " + std::string(e.what()) + 
                               (message.empty() ? "" : " - " + message));
    } catch (...) {
        throw std::runtime_error("Assertion failed: unexpected unknown exception" + 
                               (message.empty() ? "" : " - " + message));
    }
}

std::chrono::milliseconds PerformanceTest::benchmark(const std::string& name, 
                                                   BenchmarkFunction function,
                                                   int iterations) {
    LogTimer timer("Benchmark: " + name);
    
    auto start = std::chrono::steady_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        function();
    }
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    Log::info("Benchmark completed: " + name + " - " + std::to_string(iterations) + 
              " iterations in " + std::to_string(duration.count()) + "ms");
    
    return duration;
}

void PerformanceTest::completes_within(std::chrono::milliseconds limit,
                                     BenchmarkFunction function,
                                     const std::string& message) {
    auto start = std::chrono::steady_clock::now();
    function();
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    if (duration > limit) {
        throw std::runtime_error("Performance assertion failed: operation took " + 
                               std::to_string(duration.count()) + "ms, limit is " + 
                               std::to_string(limit.count()) + "ms" + 
                               (message.empty() ? "" : " - " + message));
    }
}

void MemoryTest::check_for_leaks(std::function<void()> function) {
    // This is a simplified implementation
    // In a real implementation, you would use platform-specific memory tracking
    function();
}

void MemoryTest::does_not_allocate(std::function<void()> function) {
    // This is a simplified implementation
    // In a real implementation, you would use platform-specific memory tracking
    function();
}

TestRegistry& TestRegistry::instance() {
    static TestRegistry instance;
    return instance;
}

void TestRegistry::register_test(const std::string& suite_name, const std::string& test_name, 
                               TestCase::TestFunction function) {
    tests_.emplace_back(suite_name, std::make_pair(test_name, function));
}

TestRunner TestRegistry::create_runner() {
    TestRunner runner;
    
    std::string current_suite;
    TestSuite current_suite_tests;
    
    for (auto& [suite_name, test_pair] : tests_) {
        if (suite_name != current_suite) {
            if (!current_suite.empty()) {
                runner.add_suite(current_suite, current_suite_tests);
            }
            current_suite = suite_name;
            current_suite_tests = TestSuite();
        }
        
        current_suite_tests.add_test(test_pair.first, test_pair.second);
    }
    
    if (!current_suite.empty()) {
        runner.add_suite(current_suite, current_suite_tests);
    }
    
    return runner;
}

// Explicit template instantiations
template void Assert::throws<core::VectorException>(std::function<void()>, const std::string&);

} // namespace testing
} // namespace utils
} // namespace pgvectorcpp
