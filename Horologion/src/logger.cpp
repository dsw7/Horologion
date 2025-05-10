#include "logger.hpp"

#include <iostream>
#include <mutex>
#include <unistd.h>

namespace {

const std::string LOG_INFO = " " + std::to_string(getpid()) + " I ";
const std::string LOG_WARNING = " " + std::to_string(getpid()) + " W ";
const std::string LOG_ERROR = " " + std::to_string(getpid()) + " E ";

std::string get_current_datetime_string()
{
    time_t time_now = time(0);
    struct tm time_struct;
    char time_buffer[80];

    time_struct = *localtime(&time_now);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%X", &time_struct);

    return time_buffer;
}

} // namespace

namespace logger {

void info(const std::string &message)
{
    std::cout << get_current_datetime_string() + LOG_INFO + message << '\n';
}

void warning(const std::string &message)
{
    std::cout << get_current_datetime_string() + LOG_WARNING + message << '\n';
}

void error(const std::string &message)
{
    std::cerr << get_current_datetime_string() + LOG_ERROR + message << '\n';
}

std::mutex mu;

void info_thread_safe(const std::string &message)
{
    mu.lock();
    std::cout << get_current_datetime_string() + LOG_INFO + message << '\n';
    mu.unlock();
}

void warning_thread_safe(const std::string &message)
{
    mu.lock();
    std::cout << get_current_datetime_string() + LOG_WARNING + message << '\n';
    mu.unlock();
}

void error_thread_safe(const std::string &message)
{
    mu.lock();
    std::cerr << get_current_datetime_string() + LOG_ERROR + message << '\n';
    mu.unlock();
}

} // namespace logger
