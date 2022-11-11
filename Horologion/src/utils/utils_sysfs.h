#pragma once

const std::string SYSFS_WAKEALARM = "/sys/class/rtc/rtc0/wakealarm";
const std::string SYSFS_STATE = "/sys/power/state";

bool unset_rtc_alarm();
bool set_rtc_alarm(const unsigned int wake_time);
bool is_valid_suspend_state(const std::string &state_from_ini);
bool suspend_system(const std::string &suspend_type);
