#include <iostream>
#include <string>
#include <ctime>

enum LogLevel {
    INFO,
    WARN,
    ERROR
};

class Logger
{
    public:
        Logger();
        ~Logger();
        void log(const LogLevel& level, const std::string& message);
    
    private:
        std::string get_log_level_string(const LogLevel& level);
};
