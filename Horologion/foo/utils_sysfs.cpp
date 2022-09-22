#include "utils_sysfs.h"

bool unset_rtc_alarm()
{
    Logger::info("Unsetting alarm");
    std::string unset_str = "0";

    return write_to_file(SYSFS_WAKEALARM, unset_str);
}

bool set_rtc_alarm(unsigned int wake_time)
{
    Logger::info("Setting alarm");
    std::string str_wake_time = std::to_string(wake_time);

    return write_to_file(SYSFS_WAKEALARM, str_wake_time);
}
