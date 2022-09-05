#ifndef UTILS_ALARM_H
#define UTILS_ALARM_H

#include <string>
#include <ctime>

#include "logger.h"
#include "file_utils.h"

const std::string SYSFS_WAKEALARM = "/sys/class/rtc/rtc0/wakealarm";

bool set_rtc_alarm(std::time_t wakeup_time);

#endif
