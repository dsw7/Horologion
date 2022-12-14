#include "command_base.h"

#include "logger.h"
#include "parse_config_file.h"
#include "utils_file.h"
#include "utils_sysfs.h"

#include <ctime>
#include <map>
#include <unistd.h>
#include <utility>

bool CommandBase::is_running_as_root()
{
    if (::getuid() != 0)
    {
        Logger::error("Not running as root. Additional privileges needed!");
        return false;
    }

    return true;
}

bool CommandBase::read_configs_from_file()
{
    std::string file_contents;

    if (not ::read_file(::PROG_CONFIG, file_contents))
    {
        Logger::error("Could not load configurations. Cannot continue");
        return false;
    }

    std::map<std::string, std::string> raw_configs;
    ::parse_configs(file_contents, raw_configs);

    for (auto it = raw_configs.begin(); it != raw_configs.end(); it++)
    {
        if (it->first.compare("time-wake-hour") == 0)
        {
            this->configs.time_wake.tm_hour = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-wake-minute") == 0)
        {
            this->configs.time_wake.tm_min = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-cmd-hour") == 0)
        {
            this->configs.time_run_cmd.tm_hour = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-cmd-minute") == 0)
        {
            this->configs.time_run_cmd.tm_min = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-sleep-hour") == 0)
        {
            this->configs.time_sleep.tm_hour = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-sleep-minute") == 0)
        {
            this->configs.time_sleep.tm_min = atoi(it->second.c_str());
        }
        else if (it->first.compare("suspend-type") == 0)
        {
            this->configs.suspend_type = it->second.c_str();
        }
        else if (it->first.find("target_") != std::string::npos)
        {
            std::pair<std::string, std::string> command;

            command.first = it->first;
            command.second = it->second;

            this->configs.commands.push_back(command);
        }
        else
        {
            Logger::warning("Found unknown entry in config file: \"" + it->first + "\"");
        }
    }

    Logger::info("Parsed raw configs: ");
    Logger::debug_map(raw_configs);

    return true;
}

bool CommandBase::is_config_file_input_sane()
{
    if (this->configs.time_wake.tm_hour < 0 or this->configs.time_wake.tm_hour > 23)
    {
        Logger::error("Invalid input for \"time-wake-hour\" field. Input must be between [0, 23] hours");
        return false;
    }

    if (this->configs.time_wake.tm_min < 0 or this->configs.time_wake.tm_min > 59)
    {
        Logger::error("Invalid input for \"time-wake-minute\" field. Input must be between [0, 59] minutes");
        return false;
    }

    if (this->configs.time_run_cmd.tm_hour < 0 or this->configs.time_run_cmd.tm_hour > 23)
    {
        Logger::error("Invalid input for \"time-cmd-hour\" field. Input must be between [0, 23] hours");
        return false;
    }

    if (this->configs.time_run_cmd.tm_min < 0 or this->configs.time_run_cmd.tm_min > 59)
    {
        Logger::error("Invalid input for \"time-cmd-minute\" field. Input must be between [0, 59] minutes");
        return false;
    }

    if (this->configs.time_sleep.tm_hour < 0 or this->configs.time_sleep.tm_hour > 23)
    {
        Logger::error("Invalid input for \"time-sleep-hour\" field. Input must be between [0, 23] hours");
        return false;
    }

    if (this->configs.time_sleep.tm_min < 0 or this->configs.time_sleep.tm_min > 59)
    {
        Logger::error("Invalid input for \"time-sleep-minute\" field. Input must be between [0, 59] minutes");
        return false;
    }

    Logger::info("Parsed wake up hour (tm_hour): " + std::to_string(this->configs.time_wake.tm_hour));
    Logger::info("Parsed wake up minute (tm_min): " + std::to_string(this->configs.time_wake.tm_min));

    Logger::info("Parsed command execution hour (tm_hour): " + std::to_string(this->configs.time_run_cmd.tm_hour));
    Logger::info("Parsed command execution minute (tm_min): " + std::to_string(this->configs.time_run_cmd.tm_min));

    Logger::info("Parsed sleep hour (tm_hour): " + std::to_string(this->configs.time_sleep.tm_hour));
    Logger::info("Parsed sleep minute (tm_min): " + std::to_string(this->configs.time_sleep.tm_min));

    return ::is_valid_suspend_state(this->configs.suspend_type);
}
