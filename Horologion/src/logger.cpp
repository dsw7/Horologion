#include "logger.hpp"

#include "files.hpp"

#include <fmt/core.h>
#include <fstream>
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

std::ofstream &get_file_stream()
{
    static std::ofstream fs(files::get_project_log(), std::ios::out | std::ios::app);

    if (!fs.is_open()) {
        throw std::runtime_error("Failed to open log file");
    }

    static bool first_call = true;
    if (first_call) {
        fs << fmt::format("{} {} I Beginning logging\n", get_current_datetime_string(), PID) << std::flush;
        first_call = false;
    }

    return fs;
}

} // namespace

namespace logger {

void info(const std::string &message)
{
    const std::string prefix = fmt::format("{} {} I {}\n", get_current_datetime_string(), PID, message);
    std::cout << prefix;
    std::ofstream &fs = get_file_stream();
    fs << prefix;
}

void error(const std::string &message)
{
    const std::string prefix = fmt::format("{} {} E {}\n", get_current_datetime_string(), PID, message);
    std::cerr << prefix;
    std::ofstream &fs = get_file_stream();
    fs << prefix;
}

void info_thread_safe(const std::string &message)
{
    const std::lock_guard<std::mutex> lock(LOCK);
    std::thread::id id = std::this_thread::get_id();
    std::cout << get_current_datetime_string() << " (" << id << ") " << PID << " I " << message << '\n';
}

void error_thread_safe(const std::string &message)
{
    const std::lock_guard<std::mutex> lock(LOCK);
    std::thread::id id = std::this_thread::get_id();
    std::cerr << get_current_datetime_string() << " (" << id << ") " << PID << " E " << message << '\n';
}

} // namespace logger
