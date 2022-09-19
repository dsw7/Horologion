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
        if (it->first.compare("time-alarm-hour") == 0)
        {
            this->configs.time_alarm.tm_hour = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-alarm-minute") == 0)
        {
            this->configs.time_alarm.tm_min = atoi(it->second.c_str());
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
        else if (it->first.compare("path-to-binary") == 0)
        {
            this->configs.path_to_binary = it->second.c_str();
        }
        else if (it->first.compare("path-to-log") == 0)
        {
            this->configs.path_to_log = it->second.c_str();
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

bool CommandBase::reset_rtc_alarm()
{
    Logger::info("Resetting alarm");
    std::string reset_str = "0";

    return write_to_file(SYSFS_WAKEALARM, reset_str);
}

void CommandBase::set_time_alarm()
{
    Logger::info("Parsed wake up hour (tm_hour): " + std::to_string(this->configs.time_alarm.tm_hour));
    Logger::info("Parsed wake up minute (tm_min): " + std::to_string(this->configs.time_alarm.tm_min));

    this->time_alarm = compute_epoch_wakeup_time(
        this->configs.time_alarm.tm_hour,
        this->configs.time_alarm.tm_min,
        this->configs.time_alarm.tm_sec  // set seconds to zero
    );

    Logger::info("The machine will wake up at: " + epoch_time_to_ascii_time(this->time_alarm));
}

void CommandBase::set_time_sleep()
{
    Logger::info("Parsed sleep hour (tm_hour): " + std::to_string(this->configs.time_sleep.tm_hour));
    Logger::info("Parsed sleep minute (tm_min): " + std::to_string(this->configs.time_sleep.tm_min));

    this->time_sleep = compute_epoch_sleep_time(
        this->configs.time_sleep.tm_hour,
        this->configs.time_sleep.tm_min,
        this->configs.time_sleep.tm_sec  // set seconds to zero
    );

    Logger::info("The machine will sleep at: " + epoch_time_to_ascii_time(this->time_sleep));
}

bool CommandBase::sanitize_wake_sleep_cycle()
{
    int delta_t = this->time_sleep - this->time_alarm;

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

    std::string str_wakeup_time = std::to_string(this->time_alarm);
    return write_to_file(SYSFS_WAKEALARM, str_wakeup_time);
}
