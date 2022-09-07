#include "parse_config_file.h"

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

            std::string value;

            // then we read between = and \n
            if (std::getline(is_line, value))
            {
                raw_configs[key] = value;
            }
        }
    }
}
