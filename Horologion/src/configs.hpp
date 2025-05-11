#pragma once

#include <ctime>
#include <string>
#include <vector>

struct Configs {
    std::string suspend_type;
    std::vector<std::string> commands;

    struct tm time_cmd = { 0 };
    struct tm time_sleep = { 0 };
    struct tm time_wake = { 0 };

    std::time_t time_wake_e = 0;
    std::time_t time_cmd_e = 0;
    std::time_t time_sleep_e = 0;
};

Configs read_configs_from_file();
