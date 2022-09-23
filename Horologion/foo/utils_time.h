#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <string>
#include <ctime>
#include "logger.h"

std::string epoch_time_to_ascii_time(std::time_t &epoch_time);
std::time_t get_epoch_time_from_configs(int &hour, int &minute, int &second);

#endif