#pragma once

#include <ctime>
#include <string>

namespace utils {

void is_running_as_root();
bool write_to_file(const std::string &filepath, const std::string &message);
std::string read_from_file(const std::string &filepath);

std::string epoch_time_to_ascii_time(const std::time_t &epoch_time);
std::time_t get_epoch_time_from_configs(const int &hour, const int &minute, const int &second);

bool unset_rtc_alarm();
bool set_rtc_alarm(const unsigned int wake_time);
bool is_valid_suspend_state(const std::string &state_from_ini);

} // namespace utils
