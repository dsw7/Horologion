#ifndef COMMAND_BASE_H
#define COMMAND_BASE_H

#include <string>
#include <vector>
#include <ctime>
#include <map>
#include <utility>
#include <unistd.h>

#include "logger.h"
#include "parse_config_file.h"
#include "utils_file.h"

const std::string PROG_CONFIG = "/etc/horolog.ini";

struct Configs
{
    std::vector<std::pair<std::string, std::string>> commands = {};
    std::string suspend_type = "mem";

    struct tm time_wake = {0};
    struct tm time_sleep = {0};
};

class CommandBase
{
    protected:
        std::time_t time_wake;
        std::time_t time_sleep;

        bool is_running_as_root();
        bool read_configs_from_file();
        bool is_config_file_input_sane();

    public:
        Configs configs;
        virtual bool main() {};
};

#endif
