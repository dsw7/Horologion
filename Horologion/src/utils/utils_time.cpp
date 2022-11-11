#include <string>
#include <ctime>

#include "logger.h"
#include "utils_time.h"

std::string epoch_time_to_ascii_time(std::time_t &epoch_time)
{
    std::string ascii_time = std::string(std::asctime(std::localtime(&epoch_time)));

    if (not ascii_time.empty() and ascii_time[ascii_time.length() - 1] == '\n')
    {
        ascii_time.erase(ascii_time.length() - 1);
    }

    return ascii_time;
}

std::time_t get_epoch_time_from_configs(int &hour, int &minute, int &second)
{
    std::time_t now = std::time(nullptr);
    std::tm *tm_time = std::localtime(&now);

    tm_time->tm_hour = hour;
    tm_time->tm_min = minute;
    tm_time->tm_sec = second;

    return ::mktime(tm_time);
}
