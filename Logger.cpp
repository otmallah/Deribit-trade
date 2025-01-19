#include "Logger.hpp"

Logger::Logger()
{
    log(LogLevel::INFO, "Logger created");
}

Logger::~Logger()
{
    log(LogLevel::INFO, "Logger destroyed");
}

void Logger::log(const LogLevel& level, const std::string& message)
{
    // get current time 
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    std::string current_time = std::string(buf);


    std::cout << "[" << current_time << "] " << get_log_level_string(level) << " : " << message << std::endl;
}

std::string Logger::get_log_level_string(const LogLevel& level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARN: return "WARN";
        case LogLevel::ERROR: return "ERROR";
    }
}