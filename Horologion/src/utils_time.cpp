#include "utils_time.h"

std::string epoch_time_to_ascii_time(std::time_t &epoch_time)
{
    return std::string(std::asctime(std::localtime(&epoch_time)));
}

std::time_t compute_delay(std::time_t offset_seconds)
{
    std::time_t time_since_epoch = std::time(nullptr);

    Logger::info("The current time is: ");
    epoch_time_to_ascii_time(time_since_epoch);

    std::time_t offset_time = time_since_epoch + offset_seconds;

    Logger::info("The offset time was computed as: ");
    epoch_time_to_ascii_time(offset_time);

    return offset_time;
}

std::time_t get_epoch_time_from_tm(tm &tm_time)
{
    return mktime(&tm_time);
}
