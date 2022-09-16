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
        if (it->first.compare("time-alarm-hour") == 0)
        {
            this->configs.time_alarm.tm_hour = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-alarm-minute") == 0)
        {
            this->configs.time_alarm.tm_min = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-alarm-second") == 0)
        {
            this->configs.time_alarm.tm_sec = atoi(it->second.c_str());
        }
    }

    Logger::info("Parsed wake up hour (tm_hour): " + std::to_string(this->configs.time_alarm.tm_hour));
    Logger::info("Parsed wake up minute (tm_min): " + std::to_string(this->configs.time_alarm.tm_min));
    Logger::info("Parsed wake up second (tm_sec): " + std::to_string(this->configs.time_alarm.tm_sec));
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

    std::time_t wakeup_time = get_epoch_time_from_tm(this->configs.time_alarm);

    Logger::info("Setting alarm for: " + epoch_time_to_ascii_time(wakeup_time));

    std::string str_wakeup_time = std::to_string(wakeup_time);
    write_to_file(SYSFS_WAKEALARM, str_wakeup_time);
}
