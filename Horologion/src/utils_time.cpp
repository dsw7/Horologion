#include "utils_time.h"

std::time_t get_current_epoch_time()
{
    return std::time(nullptr);
}

std::string epoch_time_to_ascii_time(std::time_t &epoch_time)
{
    return std::string(std::asctime(std::localtime(&epoch_time)));
}

std::time_t get_epoch_time_from_tm(tm &tm_time)
{
    return mktime(&tm_time);
}

std::time_t compute_epoch_wakeup_time(int &hour, int &minute, int &second)
{
    std::time_t now = get_current_epoch_time();
    std::tm *tm_time = std::localtime(&now);

    tm_time->tm_hour = hour;
    tm_time->tm_min = minute;
    tm_time->tm_sec = second;

    std::time_t wakeup_time = mktime(tm_time);

    if (wakeup_time <= now)
    {
        wakeup_time += 86400;
    }

    return wakeup_time;
}
