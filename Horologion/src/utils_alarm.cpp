#include "utils_alarm.h"

bool reset_rtc_alarm()
{
    Logger::info("Attempting to modify file " + SYSFS_WAKEALARM);

    if (not file_exists(SYSFS_WAKEALARM))
    {
        return false;
    }

    Logger::info("Resetting alarm");

    std::string reset_str = "0";
    write_to_file(SYSFS_WAKEALARM, reset_str);

    return true;
}

bool set_rtc_alarm(std::time_t wakeup_time)
{
    Logger::info("Attempting to modify file " + SYSFS_WAKEALARM);

    if (not file_exists(SYSFS_WAKEALARM))
    {
        return false;
    }

    Logger::info("Resetting alarm");

    std::string reset_str = "0";
    write_to_file(SYSFS_WAKEALARM, reset_str);

    Logger::info("Setting alarm");

    std::string str_wakeup_time = std::to_string(wakeup_time);
    write_to_file(SYSFS_WAKEALARM, str_wakeup_time);

    return true;
}
