#pragma once

#include <ctime>
#include <string>
#include <utility>
#include <vector>

struct Configs {
    std::vector<std::pair<std::string, std::string>> commands = {};
    std::string suspend_type = "mem";

    struct tm time_wake = { 0 };
    struct tm time_run_cmd = { 0 };
    struct tm time_sleep = { 0 };
};

Configs read_configs_from_file();
