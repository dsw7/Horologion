#pragma once

#include <string>

namespace logger {

void enable_file_logging();
void disable_file_logging();

void info(const std::string &message);
void error(const std::string &message);

void info_thread_safe(const std::string &message);
void error_thread_safe(const std::string &message);

} // namespace logger
