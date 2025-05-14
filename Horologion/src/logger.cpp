#include "logger.hpp"

#include "files.hpp"

#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stdlib.h>
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

void write_info(const std::string &message)
{
    std::ofstream &fs = get_file_stream();
    fs << fmt::format("{} {} I {}\n", get_current_datetime_string(), PID, message) << std::flush;
}

void write_info(const std::string &message, const std::string &tid)
{
    std::ofstream &fs = get_file_stream();
    fs << fmt::format("{} (thread {}) {} I {}\n", get_current_datetime_string(), tid, PID, message) << std::flush;
}

void write_error(const std::string &message)
{
    std::ofstream &fs = get_file_stream();
    fs << fmt::format("{} {} E {}\n", get_current_datetime_string(), PID, message) << std::flush;
}

void write_error(const std::string &message, const std::string &tid)
{
    std::ofstream &fs = get_file_stream();
    fs << fmt::format("{} (thread {}) {} E {}\n", get_current_datetime_string(), tid, PID, message) << std::flush;
}

bool should_log_to_file()
{
    const char *log_to_file = std::getenv("LOG_TO_FILE");
    if (log_to_file) {
        return true;
    }

    return false;
}

} // namespace

namespace logger {

void enable_file_logging()
{
    setenv("LOG_TO_FILE", "1", 1);
}

void info(const std::string &message)
{
    static bool log_to_file = should_log_to_file();

    if (log_to_file) {
        write_info(message);
    } else {
        fmt::print("{} {} I {}\n", get_current_datetime_string(), PID, message);
    }
}

void error(const std::string &message)
{
    static bool log_to_file = should_log_to_file();

    if (log_to_file) {
        write_error(message);
    } else {
        fmt::print(stderr, "{} {} E {}\n", get_current_datetime_string(), PID, message);
    }
}

void info_thread_safe(const std::string &message)
{
    const std::lock_guard<std::mutex> lock(LOCK);
    const std::string tid = get_thread_id();

    static bool log_to_file = should_log_to_file();

    if (log_to_file) {
        write_info(message, tid);
    } else {
        fmt::print("{} (thread {}) {} I {}\n", get_current_datetime_string(), tid, PID, message);
    }
}

void error_thread_safe(const std::string &message)
{
    const std::lock_guard<std::mutex> lock(LOCK);
    const std::string tid = get_thread_id();

    static bool log_to_file = should_log_to_file();

    if (log_to_file) {
        write_error(message, tid);
    } else {
        fmt::print(stderr, "{} (thread {}) {} E {}\n", get_current_datetime_string(), tid, PID, message);
    }
}

} // namespace logger
