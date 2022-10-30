#pragma once

#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <unistd.h>

std::string get_current_datetime_string();

const std::string LOG_INFO = " " + std::to_string(getpid()) + " I ";
const std::string LOG_WARNING = " " + std::to_string(getpid()) + " W ";
const std::string LOG_ERROR = " " + std::to_string(getpid()) + " E ";

namespace Logger
{
    void info(const std::string &message);
    void warning(const std::string &message);
    void error(const std::string &message);

    void info_thread_safe(const std::string &message);
    void warning_thread_safe(const std::string &message);
    void error_thread_safe(const std::string &message);

    void debug_map(std::map<std::string, std::string> &map);
}
