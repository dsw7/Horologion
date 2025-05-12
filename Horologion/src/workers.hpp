#pragma once

#include <ctime>
#include <string>

namespace workers {
void stay_awake(const std::time_t duration, const std::string &suspend_type);
void run_command(const std::string &command);
} // namespace workers
