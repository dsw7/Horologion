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
};

Configs read_configs_from_file();
