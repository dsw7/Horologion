#ifndef PARSE_CONFIG_FILE_H
#define PARSE_CONFIG_FILE_H

#include <string>
#include <map>
#include <sstream>

void parse_configs(std::string &file_contents, std::map<std::string, std::string> &raw_configs);

#endif
