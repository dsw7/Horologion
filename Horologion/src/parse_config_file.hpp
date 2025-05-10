#pragma once

#include <ctime>
#include <map>
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

void parse_configs(const std::string &file_contents, std::map<std::string, std::string> &raw_configs);
