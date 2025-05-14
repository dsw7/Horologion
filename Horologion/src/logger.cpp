#include "logger.hpp"

#include "files.hpp"

#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
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

std::string get_thread_id()
{
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    return oss.str();
}

void log_to_stdout(const std::string &message)
{
    fmt::print("{} {} I {}\n", get_current_datetime_string(), PID, message);
}

void log_to_stdout(const std::string &message, const std::string &tid)
{
    fmt::print("{} (thread {}) {} I {}\n", get_current_datetime_string(), tid, PID, message);
}

void log_to_stderr(const std::string &message)
{
    fmt::print(stderr, "{} {} E {}\n", get_current_datetime_string(), PID, message);
}

void log_to_stderr(const std::string &message, const std::string &tid)
{
    fmt::print(stderr, "{} (thread {}) {} E {}\n", get_current_datetime_string(), tid, PID, message);
}

} // namespace

namespace logger {

void info(const std::string &message)
{
    log_to_stdout(message);

    std::ofstream &fs = get_file_stream();
    const std::string prefix = fmt::format("{} {} I {}\n", get_current_datetime_string(), PID, message);
    fs << prefix;
}

void error(const std::string &message)
{
    log_to_stderr(message);

    const std::string prefix = fmt::format("{} {} E {}\n", get_current_datetime_string(), PID, message);
    std::ofstream &fs = get_file_stream();
    fs << prefix;
}

void info_thread_safe(const std::string &message)
{
    const std::lock_guard<std::mutex> lock(LOCK);
    const std::string tid = get_thread_id();
    log_to_stdout(message, tid);
}

void error_thread_safe(const std::string &message)
{
    const std::lock_guard<std::mutex> lock(LOCK);
    const std::string tid = get_thread_id();
    log_to_stderr(message, tid);
}

} // namespace logger
