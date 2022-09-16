#include "utils_time.h"

std::string epoch_time_to_ascii_time(std::time_t &epoch_time)
{
    return std::string(std::asctime(std::localtime(&epoch_time)));
}

std::time_t get_epoch_time_from_tm(tm &tm_time)
{
    return mktime(&tm_time);
}
