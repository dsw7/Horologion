#pragma once

#include <map>
#include <string>

void parse_configs(const std::string &file_contents, std::map<std::string, std::string> &raw_configs);
