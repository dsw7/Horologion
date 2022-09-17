#include "logger.h"

std::string get_current_datetime_string()
{
    time_t time_now = time(0);
    struct tm time_struct;
    char   time_buffer[80];

    time_struct = *localtime(&time_now);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%X", &time_struct);

    return time_buffer;
}

namespace Logger {

    void info(const std::string &message)
    {
        std::cout << get_current_datetime_string() + " I " + message << std::endl;
    }

    void warning(const std::string &message)
    {
        std::cout << get_current_datetime_string() + " W " + message << std::endl;
    }

    void error(const std::string &message)
    {
        std::cerr << get_current_datetime_string() + " E " + message << std::endl;
    }

    void debug_map(std::map<std::string, std::string> &map)
    {
        std::map<std::string, std::string>::iterator it = map.begin();

        std::cout << get_current_datetime_string() + " I {\n";
        while (it != map.end())
        {
            std::cout << "    \"" << it->first << "\": \"" << it->second << "\",\n";
            it++;
        }
        std::cout << "}" << std::endl;

    }
}
