/**
 * Logging framework for pgvectorcpp
 * 
 * This header provides a flexible logging framework with
 * multiple log levels and output destinations.
 */

#ifndef LOGGING_H
#define LOGGING_H

#include <string>
#include <memory>
#include <mutex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <vector>

namespace pgvectorcpp {
namespace utils {

/**
 * Log levels
 */
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3,
    FATAL = 4
};

/**
 * Base logger interface
 */
class Logger {
public:
    virtual ~Logger() = default;
    
    /**
     * Log a message
     */
    virtual void log(LogLevel level, const std::string& message) = 0;
    
    /**
     * Set minimum log level
     */
    void set_level(LogLevel level) { min_level_ = level; }
    
    /**
     * Check if level is enabled
     */
    bool is_enabled(LogLevel level) const { return level >= min_level_; }
    
protected:
    LogLevel min_level_ = LogLevel::INFO;
    
    /**
     * Format log message with timestamp and level
     */
    std::string format_message(LogLevel level, const std::string& message);
};

/**
 * Console logger
 */
class ConsoleLogger : public Logger {
public:
    void log(LogLevel level, const std::string& message) override;
};

/**
 * File logger
 */
class FileLogger : public Logger {
public:
    explicit FileLogger(const std::string& filename);
    ~FileLogger();
    
    void log(LogLevel level, const std::string& message) override;
    
private:
    std::ofstream file_;
    std::mutex mutex_;
};

/**
 * Composite logger that can log to multiple destinations
 */
class CompositeLogger : public Logger {
public:
    void add_logger(std::shared_ptr<Logger> logger);
    void log(LogLevel level, const std::string& message) override;
    
private:
    std::vector<std::shared_ptr<Logger>> loggers_;
    std::mutex mutex_;
};

/**
 * Global logger instance
 */
class Log {
public:
    /**
     * Initialize logging with a logger
     */
    static void init(std::shared_ptr<Logger> logger);
    
    /**
     * Get the current logger
     */
    static std::shared_ptr<Logger> get_logger();
    
    /**
     * Log a message
     */
    static void log(LogLevel level, const std::string& message);
    
    /**
     * Convenience methods for different log levels
     */
    static void debug(const std::string& message) { log(LogLevel::DEBUG, message); }
    static void info(const std::string& message) { log(LogLevel::INFO, message); }
    static void warning(const std::string& message) { log(LogLevel::WARNING, message); }
    static void error(const std::string& message) { log(LogLevel::ERROR, message); }
    static void fatal(const std::string& message) { log(LogLevel::FATAL, message); }
    
    /**
     * Set minimum log level
     */
    static void set_level(LogLevel level);
    
private:
    static std::shared_ptr<Logger> logger_;
    static std::mutex mutex_;
};

/**
 * RAII log timer for performance measurement
 */
class LogTimer {
public:
    LogTimer(const std::string& operation, LogLevel level = LogLevel::DEBUG);
    ~LogTimer();
    
private:
    std::string operation_;
    LogLevel level_;
    std::chrono::steady_clock::time_point start_time_;
};

/**
 * Macro for logging with file and line information
 */
#define LOG_DEBUG(msg) pgvectorcpp::utils::Log::debug(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + msg)
#define LOG_INFO(msg) pgvectorcpp::utils::Log::info(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + msg)
#define LOG_WARNING(msg) pgvectorcpp::utils::Log::warning(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + msg)
#define LOG_ERROR(msg) pgvectorcpp::utils::Log::error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + msg)
#define LOG_FATAL(msg) pgvectorcpp::utils::Log::fatal(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + msg)

} // namespace utils
} // namespace pgvectorcpp

#endif // LOGGING_H
