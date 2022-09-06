#include "commands.h"

bool CommandBase::is_running_as_root()
{
    if (getuid() != 0)
    {
        Logger::error("Not running as root. Additional privileges needed!");
        return false;
    }

    return true;
}

bool CommandBase::wakealarm_exists()
{
    Logger::info("Checking if wakealarm file exists: " + SYSFS_WAKEALARM);

    if (not file_exists(SYSFS_WAKEALARM))
    {
        return false;
    }
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

bool CommandResetAlarm::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    if (not this->wakealarm_exists())
    {
        return false;
    }

    reset_rtc_alarm();
    return true;
};

bool CommandSetAlarm::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    if (not this->wakealarm_exists())
    {
        return false;
    }

    return true;
};
