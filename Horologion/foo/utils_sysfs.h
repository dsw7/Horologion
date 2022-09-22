#ifndef UTILS_SYSFS_H
#define UTILS_SYSFS_H

#include <string>

#include "utils_file.h"
#include "logger.h"

const std::string SYSFS_WAKEALARM = "/sys/class/rtc/rtc0/wakealarm";

bool unset_rtc_alarm();
bool set_rtc_alarm(std::string &wake_time);

#endif
