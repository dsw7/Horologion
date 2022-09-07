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

bool CommandBase::config_exists()
{
    Logger::info("Checking if program configuration file exists: " + PROG_CONFIG);

    if (not file_exists(PROG_CONFIG))
    {
        return false;
    }

    return true;
}

void CommandBase::read_configs_from_file()
{
    std::string file_contents;

    read_file(PROG_CONFIG, file_contents);

    std::map<std::string, std::string> raw_configs;
    parse_configs(file_contents, raw_configs);

    for (auto it = raw_configs.begin(); it != raw_configs.end(); it++)
    {
        if (it->first.compare("time_alarm_year") == 0)
        {
            this->configs.time_alarm.tm_year = atoi(it->second.c_str());
        }
        else if (it->first.compare("time_alarm_month") == 0)
        {
            this->configs.time_alarm.tm_mon = atoi(it->second.c_str());
        }
        else if (it->first.compare("time_alarm_day") == 0)
        {
            this->configs.time_alarm.tm_mday = atoi(it->second.c_str());
        }
        else if (it->first.compare("time_alarm_hour") == 0)
        {
            this->configs.time_alarm.tm_hour = atoi(it->second.c_str());
        }
        else if (it->first.compare("time_alarm_minute") == 0)
        {
            this->configs.time_alarm.tm_min = atoi(it->second.c_str());
        }
        else if (it->first.compare("time_alarm_second") == 0)
        {
            this->configs.time_alarm.tm_sec = atoi(it->second.c_str());
        }
    }
}

bool CommandBase::wakealarm_exists()
{
    Logger::info("Checking if wakealarm file exists: " + SYSFS_WAKEALARM);

    if (not file_exists(SYSFS_WAKEALARM))
    {
        return false;
    }

    return true;
}

void CommandBase::reset_rtc_alarm()
{
    Logger::info("Resetting alarm");

    std::string reset_str = "0";
    write_to_file(SYSFS_WAKEALARM, reset_str);
}

void CommandBase::set_rtc_alarm()
{
    this->reset_rtc_alarm();

    Logger::info("Setting alarm");

    std::time_t wakeup_time = compute_delay(60);

    std::string str_wakeup_time = std::to_string(wakeup_time);
    write_to_file(SYSFS_WAKEALARM, str_wakeup_time);
}
