#include "command_base.h"

bool CommandBase::is_running_as_root()
{
    if (getuid() != 0)
    {
        Logger::error("Not running as root. Additional privileges needed!");
        return false;
    }

    return true;
}

bool CommandBase::read_configs_from_file()
{
    std::string file_contents;

    if (not read_file(PROG_CONFIG, file_contents))
    {
        Logger::error("Could not load configurations. Cannot continue");
        return false;
    }

    std::map<std::string, std::string> raw_configs;
    parse_configs(file_contents, raw_configs);

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

    return true;
}

bool CommandBase::reset_rtc_alarm()
{
    Logger::info("Resetting alarm");
    std::string reset_str = "0";

    return write_to_file(SYSFS_WAKEALARM, reset_str);
}

void CommandBase::compute_wake_sleep_window()
{
    Logger::info("Parsed wake up hour (tm_hour): " + std::to_string(this->configs.time_wake.tm_hour));
    Logger::info("Parsed wake up minute (tm_min): " + std::to_string(this->configs.time_wake.tm_min));

    Logger::info("Parsed sleep hour (tm_hour): " + std::to_string(this->configs.time_sleep.tm_hour));
    Logger::info("Parsed sleep minute (tm_min): " + std::to_string(this->configs.time_sleep.tm_min));

    this->time_wake = get_epoch_time_from_configs(
        this->configs.time_wake.tm_hour,
        this->configs.time_wake.tm_min,
        this->configs.time_wake.tm_sec  // set seconds to zero
    );

    this->time_sleep = get_epoch_time_from_configs(
        this->configs.time_sleep.tm_hour,
        this->configs.time_sleep.tm_min,
        this->configs.time_sleep.tm_sec  // set seconds to zero
    );

    if (wake_time_is_earlier_than_current_time(this->time_wake))
    {
        this->time_wake += SECONDS_PER_DAY;
        this->time_sleep += SECONDS_PER_DAY;
    }

    Logger::info("The machine will wake up at: " + epoch_time_to_ascii_time(this->time_wake));
    Logger::info("The machine will sleep at: " + epoch_time_to_ascii_time(this->time_sleep));
}

bool CommandBase::sanitize_wake_sleep_cycle()
{
    int delta_t = this->time_sleep - this->time_wake;

    if (delta_t < 0)
    {
        Logger::error("Sleep time must be greater than wake time!");
        return false;
    }

    if (delta_t >= 0 and delta_t < MINIMUM_WAKE_TIME)
    {
        Logger::error("The wake-sleep duration must be at least " + std::to_string(MINIMUM_WAKE_TIME) + " s");
        return false;
    }

    Logger::info("The wake-sleep duration will be " + std::to_string(delta_t) + " s");
    return true;
}

bool CommandBase::set_rtc_alarm()
{
    if (not this->reset_rtc_alarm())
    {
        Logger::error("Failed to reset alarm");
        return false;
    }

    std::string str_wakeup_time = std::to_string(this->time_wake);
    return write_to_file(SYSFS_WAKEALARM, str_wakeup_time);
}
