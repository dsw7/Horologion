#include "utils_time.h"

std::time_t get_current_epoch_time()
{
    return std::time(nullptr);
}

std::string epoch_time_to_ascii_time(std::time_t &epoch_time)
{
    std::string ascii_time = std::string(std::asctime(std::localtime(&epoch_time)));

    if (not ascii_time.empty() and ascii_time[ascii_time.length() - 1] == '\n')
    {
        ascii_time.erase(ascii_time.length() - 1);
    }

    return ascii_time;
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
        // i.e. wake up next day if we already passed today's wake up time
        wakeup_time += 86400;
    }

    return wakeup_time;
}

std::time_t compute_epoch_sleep_time(int &hour, int &minute, int &second)
{
    std::time_t now = get_current_epoch_time();
    std::tm *tm_time = std::localtime(&now);

    tm_time->tm_hour = hour;
    tm_time->tm_min = minute;
    tm_time->tm_sec = second;

    std::time_t sleep_time = mktime(tm_time);

    if (sleep_time <= now)
    {
        // i.e. sleep next day if we already passed today's sleep time
        sleep_time += 86400;
    }

    return sleep_time;
}
