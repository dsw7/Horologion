#include "utils_sysfs.h"

bool unset_rtc_alarm()
{
    Logger::info("Unsetting alarm");
    std::string unset_str = "0";

    return write_to_file(SYSFS_WAKEALARM, unset_str);
}

bool set_rtc_alarm(std::string &wake_time)
{
    Logger::info("Setting alarm");

    if (unset_rtc_alarm())
    {
        Logger::error("Failed to unset alarm");
        return false;
    }

    return write_to_file(SYSFS_WAKEALARM, wake_time);
}
