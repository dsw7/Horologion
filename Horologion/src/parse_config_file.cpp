#include "parse_config_file.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <map>
#include <sstream>
#include <stdexcept>

namespace {

const std::string PROG_CONFIG = "/etc/horolog.ini";

void strip_whitespace_from_left(std::string &str)
{
    const std::size_t found_ws = str.find_first_not_of(" ");

    if (found_ws == std::string::npos) {
        return;
    }

    str.erase(0, found_ws);
}

void strip_whitespace_from_right(std::string &str)
{
    const std::size_t found_ws = str.find_last_not_of(" ");

    if (found_ws == std::string::npos) {
        return;
    }

    str.erase(found_ws + 1);
}

void parse_configs(const std::string &file_contents, std::map<std::string, std::string> &raw_configs)
{
    std::istringstream is_file(file_contents);
    std::string line;

    while (std::getline(is_file, line)) // we read line by line - up to \n in this case
    {
        std::istringstream is_line(line);

        std::size_t is_comment = line.find("#");
        if (is_comment != std::string::npos) {
            continue;
        }

        std::string key;

        // then for the extracted line, is_line, we read from the beginning of the line to =
        if (std::getline(is_line, key, '=')) {
            strip_whitespace_from_left(key);
            strip_whitespace_from_right(key);

            std::string value;

            // then we read between = and \n
            if (std::getline(is_line, value)) {
                strip_whitespace_from_left(value);
                strip_whitespace_from_right(value);

                raw_configs[key] = value;
            }
        }
    }
}

void is_config_file_input_sane(const Configs &configs)
{
    if (configs.time_wake.tm_hour < 0 or configs.time_wake.tm_hour > 23) {
        throw std::runtime_error("Invalid input for \"time-wake-hour\" field. Input must be between [0, 23] hours");
    }

    if (configs.time_wake.tm_min < 0 or configs.time_wake.tm_min > 59) {
        throw std::runtime_error("Invalid input for \"time-wake-minute\" field. Input must be between [0, 59] minutes");
    }

    if (configs.time_run_cmd.tm_hour < 0 or configs.time_run_cmd.tm_hour > 23) {
        throw std::runtime_error("Invalid input for \"time-cmd-hour\" field. Input must be between [0, 23] hours");
    }

    if (configs.time_run_cmd.tm_min < 0 or configs.time_run_cmd.tm_min > 59) {
        throw std::runtime_error("Invalid input for \"time-cmd-minute\" field. Input must be between [0, 59] minutes");
    }

    if (configs.time_sleep.tm_hour < 0 or configs.time_sleep.tm_hour > 23) {
        throw std::runtime_error("Invalid input for \"time-sleep-hour\" field. Input must be between [0, 23] hours");
    }

    if (configs.time_sleep.tm_min < 0 or configs.time_sleep.tm_min > 59) {
        throw std::runtime_error("Invalid input for \"time-sleep-minute\" field. Input must be between [0, 59] minutes");
    }

    logger::info("Parsed wake up hour (tm_hour): " + std::to_string(configs.time_wake.tm_hour));
    logger::info("Parsed wake up minute (tm_min): " + std::to_string(configs.time_wake.tm_min));

    logger::info("Parsed command execution hour (tm_hour): " + std::to_string(configs.time_run_cmd.tm_hour));
    logger::info("Parsed command execution minute (tm_min): " + std::to_string(configs.time_run_cmd.tm_min));

    logger::info("Parsed sleep hour (tm_hour): " + std::to_string(configs.time_sleep.tm_hour));
    logger::info("Parsed sleep minute (tm_min): " + std::to_string(configs.time_sleep.tm_min));

    if (not utils::is_valid_suspend_state(configs.suspend_type)) {
        throw std::runtime_error("Invalid suspend type");
    }
}

} // namespace

Configs read_configs_from_file()
{
    std::string file_contents;

    if (not utils::read_file(PROG_CONFIG, file_contents)) {
        throw std::runtime_error("Could not load configurations. Cannot continue");
    }

    std::map<std::string, std::string> raw_configs;
    parse_configs(file_contents, raw_configs);

    logger::info("Parsed raw configs: ");
    logger::debug_map(raw_configs);

    Configs configs;

    for (auto it = raw_configs.begin(); it != raw_configs.end(); it++) {
        if (it->first == "time-wake-hour") {
            configs.time_wake.tm_hour = atoi(it->second.c_str());
        } else if (it->first == "time-wake-minute") {
            configs.time_wake.tm_min = atoi(it->second.c_str());
        } else if (it->first == "time-cmd-hour") {
            configs.time_run_cmd.tm_hour = atoi(it->second.c_str());
        } else if (it->first == "time-cmd-minute") {
            configs.time_run_cmd.tm_min = atoi(it->second.c_str());
        } else if (it->first == "time-sleep-hour") {
            configs.time_sleep.tm_hour = atoi(it->second.c_str());
        } else if (it->first == "time-sleep-minute") {
            configs.time_sleep.tm_min = atoi(it->second.c_str());
        } else if (it->first == "suspend-type") {
            configs.suspend_type = it->second.c_str();
        } else if (it->first.find("target_") != std::string::npos) {
            std::pair<std::string, std::string> command;
            command.first = it->first;
            command.second = it->second;
            configs.commands.push_back(command);
        } else {
            logger::warning("Found unknown entry in config file: \"" + it->first + "\"");
        }
    }

    is_config_file_input_sane(configs);

    return configs;
}
