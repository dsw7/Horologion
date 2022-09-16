#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <string>
#include <ctime>
#include <time.h>
#include "logger.h"

std::time_t get_current_epoch_time();
std::string epoch_time_to_ascii_time(std::time_t &epoch_time);

std::time_t compute_epoch_wakeup_time(int &hour, int &minute, int &second);
std::time_t compute_epoch_sleep_time(int &hour, int &minute, int &second);

#endif
