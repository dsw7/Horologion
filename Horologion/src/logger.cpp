#include "logger.hpp"

#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>

namespace {

std::mutex LOCK;
const pid_t PID = getpid();

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
    std::cout << get_current_datetime_string() << " " << PID << " I " << message << '\n';
}

void warning(const std::string &message)
{
    std::cout << get_current_datetime_string() << " " << PID << " W " << message << '\n';
}

void error(const std::string &message)
{
    std::cerr << get_current_datetime_string() << " " << PID << " E " << message << '\n';
}

void info_thread_safe(const std::string &message)
{
    const std::lock_guard<std::mutex> lock(LOCK);
    std::thread::id id = std::this_thread::get_id();
    std::cout << get_current_datetime_string() << " (" << id << ") " << PID << " I " << message << '\n';
}

void warning_thread_safe(const std::string &message)
{
    const std::lock_guard<std::mutex> lock(LOCK);
    std::thread::id id = std::this_thread::get_id();
    std::cout << get_current_datetime_string() << " (" << id << ") " << PID << " W " << message << '\n';
}

void error_thread_safe(const std::string &message)
{
    const std::lock_guard<std::mutex> lock(LOCK);
    std::thread::id id = std::this_thread::get_id();
    std::cerr << get_current_datetime_string() << " (" << id << ") " << PID << " E " << message << '\n';
}

} // namespace logger
