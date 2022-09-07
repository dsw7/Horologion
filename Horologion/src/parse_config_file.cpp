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

bool read_configs_from_file(std::string path_config_file, Configs &configs)
{
    if (!file_exists(path_config_file))
    {
        std::cerr << "Path " + path_config_file + " does not exist!" << std::endl;
        return false;
    }

    std::string file_contents;
    read_file(path_config_file, file_contents);

    std::map<std::string, std::string> raw_configs;
    parse_configs(file_contents, raw_configs);

    std::map<std::string, std::string>::iterator it;
    for (it = raw_configs.begin(); it != raw_configs.end(); it++)
    {
        if (it->first.compare("port") == 0)
        {
            configs.tcp_port = atoi(it->second.c_str());
        }
        else if (it->first.compare("buffer-size") == 0)
        {
            configs.tcp_buffer_size = atoi(it->second.c_str());
        }
        else if (it->first.compare("bind-ip") == 0)
        {
            configs.bind_ip = it->second;
        }
        else if (it->first.compare("backlog") == 0)
        {
            configs.backlog = atoi(it->second.c_str());
        }
        else if (it->first.compare("master") == 0)
        {
            configs.master = it->second;
        }
    }

    return true;
}
