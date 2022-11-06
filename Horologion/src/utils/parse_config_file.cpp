#include <string>
#include <map>
#include <sstream>

#include "parse_config_file.h"

void strip_whitespace_from_left(std::string &str)
{
    std::size_t found_ws = str.find_first_not_of(" ");

    if (found_ws == std::string::npos)
    {
        return;
    }

    str.erase(0, found_ws);
}

void strip_whitespace_from_right(std::string &str)
{
    std::size_t found_ws = str.find_last_not_of(" ");

    if (found_ws == std::string::npos)
    {
        return;
    }

    str.erase(found_ws + 1);
}

void parse_configs(std::string &file_contents, std::map<std::string, std::string> &raw_configs)
{
    std::istringstream is_file(file_contents);
    std::string line;

    while (std::getline(is_file, line))  // we read line by line - up to \n in this case
    {
        std::istringstream is_line(line);

        std::size_t is_comment = line.find("#");
        if (is_comment != std::string::npos)
        {
            continue;
        }

        std::string key;

        // then for the extracted line, is_line, we read from the beginning of the line to =
        if (std::getline(is_line, key, '='))
        {
            strip_whitespace_from_left(key);
            strip_whitespace_from_right(key);

            std::string value;

            // then we read between = and \n
            if (std::getline(is_line, value))
            {
                strip_whitespace_from_left(value);
                strip_whitespace_from_right(value);

                raw_configs[key] = value;
            }
        }
    }
}
