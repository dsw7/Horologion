#pragma once

#include <ctime>
#include <string>
#include <vector>

const std::string PROG_CONFIG = "/etc/horolog.ini";

struct Configs {
    std::vector<std::pair<std::string, std::string>> commands = {};
    std::string suspend_type = "mem";

    struct tm time_wake = { 0 };
    struct tm time_run_cmd = { 0 };
    struct tm time_sleep = { 0 };
};

class CommandBase {
public:
    Configs configs;
    virtual void main() {};

protected:
    bool is_running_as_root();
    bool read_configs_from_file();
    bool is_config_file_input_sane();
};
