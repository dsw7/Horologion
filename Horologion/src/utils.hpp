#pragma once

#include <ctime>
#include <string>

namespace utils {

void is_running_as_root();
void write_to_file(const std::string &filepath, const std::string &contents);
std::string read_from_file(const std::string &filepath);

std::string epoch_time_to_ascii_time(const std::time_t &epoch_time);
} // namespace utils
