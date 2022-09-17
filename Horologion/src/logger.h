#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <map>
#include <time.h>

std::string get_current_datetime_string();

namespace Logger
{
    void info(const std::string &message);
    void warning(const std::string &message);
    void error(const std::string &message);
    void debug_map(std::map<std::string, std::string> &map);
}

#endif
