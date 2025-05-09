#include "logger.hpp"

#include <iostream>
#include <mutex>

// return pointer
std::string get_current_datetime_string()
{
    time_t time_now = time(0);
    struct tm time_struct;
    char time_buffer[80];

    time_struct = *::localtime(&time_now);
    ::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%X", &time_struct);

    return time_buffer;
}

namespace Logger {

void info(const std::string &message)
{
    std::cout << ::get_current_datetime_string() + ::LOG_INFO + message << std::endl;
}

void warning(const std::string &message)
{
    std::cout << ::get_current_datetime_string() + ::LOG_WARNING + message << std::endl;
}

void error(const std::string &message)
{
    std::cerr << ::get_current_datetime_string() + ::LOG_ERROR + message << std::endl;
}

std::mutex mu;

void info_thread_safe(const std::string &message)
{
    mu.lock();
    std::cout << ::get_current_datetime_string() + ::LOG_INFO + message << std::endl;
    mu.unlock();
}

void warning_thread_safe(const std::string &message)
{
    mu.lock();
    std::cout << ::get_current_datetime_string() + ::LOG_WARNING + message << std::endl;
    mu.unlock();
}

void error_thread_safe(const std::string &message)
{
    mu.lock();
    std::cerr << ::get_current_datetime_string() + ::LOG_ERROR + message << std::endl;
    mu.unlock();
}

void debug_map(std::map<std::string, std::string> &map)
{
    std::map<std::string, std::string>::iterator it = map.begin();

    std::cout << ::get_current_datetime_string() + ::LOG_INFO + "{\n";
    while (it != map.end()) {
        std::cout << "    \"" << it->first << "\": \"" << it->second << "\",\n";
        it++;
    }
    std::cout << "}" << std::endl;
}
} // namespace Logger
