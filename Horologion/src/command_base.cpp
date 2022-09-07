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
        if (it->first.compare("time-alarm-year") == 0)
        {
            this->configs.time_alarm.tm_year = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-alarm-month") == 0)
        {
            this->configs.time_alarm.tm_mon = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-alarm-day") == 0)
        {
            this->configs.time_alarm.tm_mday = atoi(it->second.c_str());
        }
        else if (it->first.compare("time-alarm-hour") == 0)
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

    Logger::info("Parsed year (tm_year): " + std::to_string(this->configs.time_alarm.tm_year));
    Logger::info("Parsed month (tm_mon): " + std::to_string(this->configs.time_alarm.tm_mon));
    Logger::info("Parsed day (tm_mday): " + std::to_string(this->configs.time_alarm.tm_mday));
    Logger::info("Parsed hour (tm_hour): " + std::to_string(this->configs.time_alarm.tm_hour));
    Logger::info("Parsed minute (tm_min): " + std::to_string(this->configs.time_alarm.tm_min));
    Logger::info("Parsed month (tm_sec): " + std::to_string(this->configs.time_alarm.tm_sec));
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

    std::time_t wakeup_time = get_epoch_time_from_tm(this->configs.time_alarm);
    //std::time_t wakeup_time = compute_delay(60);

    std::string str_wakeup_time = std::to_string(wakeup_time);
    write_to_file(SYSFS_WAKEALARM, str_wakeup_time);
}
