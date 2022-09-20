#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <string>
#include <ctime>
#include <time.h>
#include "logger.h"

std::time_t get_current_epoch_time();
std::string epoch_time_to_ascii_time(std::time_t &epoch_time);
std::time_t get_epoch_time_from_configs(int &hour, int &minute, int &second);
bool wake_time_is_earlier_than_current_time(std::time_t &wake_time);

#endif
