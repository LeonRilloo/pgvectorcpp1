#include "logging.hpp"

namespace pgvectorcpp {
namespace utils {

std::shared_ptr<Logger> Log::logger_;
std::mutex Log::mutex_;

std::string Logger::format_message(LogLevel level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    
    const char* level_str = nullptr;
    switch (level) {
        case LogLevel::DEBUG: level_str = "DEBUG"; break;
        case LogLevel::INFO: level_str = "INFO"; break;
        case LogLevel::WARNING: level_str = "WARNING"; break;
        case LogLevel::ERROR: level_str = "ERROR"; break;
        case LogLevel::FATAL: level_str = "FATAL"; break;
    }
    
    ss << " [" << level_str << "] " << message;
    return ss.str();
}

void ConsoleLogger::log(LogLevel level, const std::string& message) {
    if (!is_enabled(level)) return;
    
    std::ostream& out = (level >= LogLevel::ERROR) ? std::cerr : std::cout;
    out << format_message(level, message) << std::endl;
}

FileLogger::FileLogger(const std::string& filename) {
    file_.open(filename, std::ios::app);
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filename);
    }
}

FileLogger::~FileLogger() {
    if (file_.is_open()) {
        file_.close();
    }
}

void FileLogger::log(LogLevel level, const std::string& message) {
    if (!is_enabled(level)) return;
    
    std::lock_guard<std::mutex> lock(mutex_);
    file_ << format_message(level, message) << std::endl;
    file_.flush();
}

void CompositeLogger::add_logger(std::shared_ptr<Logger> logger) {
    std::lock_guard<std::mutex> lock(mutex_);
    loggers_.push_back(logger);
}

void CompositeLogger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& logger : loggers_) {
        logger->log(level, message);
    }
}

void Log::init(std::shared_ptr<Logger> logger) {
    std::lock_guard<std::mutex> lock(mutex_);
    logger_ = logger;
}

std::shared_ptr<Logger> Log::get_logger() {
    std::lock_guard<std::mutex> lock(mutex_);
    return logger_;
}

void Log::log(LogLevel level, const std::string& message) {
    auto logger = get_logger();
    if (logger) {
        logger->log(level, message);
    }
}

void Log::set_level(LogLevel level) {
    auto logger = get_logger();
    if (logger) {
        logger->set_level(level);
    }
}

LogTimer::LogTimer(const std::string& operation, LogLevel level)
    : operation_(operation), level_(level), start_time_(std::chrono::steady_clock::now()) {
    Log::log(level_, "Started: " + operation_);
}

LogTimer::~LogTimer() {
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_).count();
    Log::log(level_, "Completed: " + operation_ + " (" + std::to_string(duration) + "ms)");
}

} // namespace utils
} // namespace pgvectorcpp
