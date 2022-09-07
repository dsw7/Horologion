#ifndef UTILS_TIME_H
#define UTILS_TIME_H

#include <ctime>
#include <time.h>
#include "logger.h"

std::time_t compute_delay(std::time_t offset_seconds);
std::time_t get_epoch_time_from_tm(tm &time);

#endif
