#pragma once

#include <map>
#include <string>

namespace logger {

void info(const std::string &message);
void warning(const std::string &message);
void error(const std::string &message);

void info_thread_safe(const std::string &message);
void warning_thread_safe(const std::string &message);
void error_thread_safe(const std::string &message);

void debug_map(std::map<std::string, std::string> &map);

} // namespace logger
