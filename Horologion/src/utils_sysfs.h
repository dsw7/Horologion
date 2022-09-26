#ifndef UTILS_SYSFS_H
#define UTILS_SYSFS_H

#include <map>
#include <iostream>
#include <string>
#include <sstream>

#include "utils_file.h"
#include "logger.h"

const std::string SYSFS_WAKEALARM = "/sys/class/rtc/rtc0/wakealarm";
const std::string SYSFS_STATE = "/sys/power/state";

bool unset_rtc_alarm();
bool set_rtc_alarm(unsigned int wake_time);
bool is_valid_suspend_state(std::string &state_from_ini);
bool suspend_system(std::string &suspend_type);

#endif
