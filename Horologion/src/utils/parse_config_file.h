#pragma once

#include <map>
#include <string>

void strip_whitespace_from_left(std::string &str);
void strip_whitespace_from_right(std::string &str);
void parse_configs(std::string &file_contents, std::map<std::string, std::string> &raw_configs);
