#pragma once

#include <ctime>
#include <string>

namespace workers {
void worker_stay_awake(const std::time_t *duration, std::string *suspend_type);
void run_command(const std::string &command);
} // namespace workers
