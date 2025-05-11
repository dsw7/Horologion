#include "configs.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <filesystem>
#include <stdexcept>
#include <stdlib.h>
#include <toml.hpp>

namespace {

std::filesystem::path get_proj_data_dir()
{
    // i.e. program running via systemd and the HOROLOG_DATADIR
    // env var was set by Environment= directive
    const char *horolog_datadir = std::getenv("HOROLOG_DATADIR");
    if (horolog_datadir) {
        return std::filesystem::path(horolog_datadir);
    }

    // i.e. program running as sudoer and the HOROLOG_DATADIR env var is unset
    const char *sudo_user = std::getenv("SUDO_USER");
    if (sudo_user) {
        return std::filesystem::path("/home") / sudo_user / ".horolog";
    }

    // If all else fails... try to read HOME env var
    const char *home_dir = std::getenv("HOME");
    if (not home_dir) {
        throw std::runtime_error("Could not locate user home directory!");
    }

    return std::filesystem::path(home_dir) / ".horolog";
}

void read_project_toml(Configs &configs)
{
    static std::string prog_config = get_proj_data_dir() / "horolog.toml";

    if (not std::filesystem::exists(prog_config)) {
        throw std::runtime_error("Configuration file \"" + prog_config + "\" does not exist");
    }

    toml::table table = toml::parse_file(prog_config);
    configs.time_wake.tm_hour = table["times"]["wake"]["hour"].value_or<int>(8);
    configs.time_wake.tm_min = table["times"]["wake"]["minute"].value_or<int>(0);
    configs.time_cmd.tm_hour = table["times"]["cmd"]["hour"].value_or<int>(8);
    configs.time_cmd.tm_min = table["times"]["cmd"]["minute"].value_or<int>(1);
    configs.time_sleep.tm_hour = table["times"]["sleep"]["hour"].value_or<int>(8);
    configs.time_sleep.tm_min = table["times"]["sleep"]["minute"].value_or<int>(2);
    configs.suspend_type = table["suspend-type"].value_or<std::string>("mem");

    const auto &targets = table["targets"];
    if (targets.is_array()) {
        for (auto it = targets.as_array()->begin(); it < targets.as_array()->end(); ++it) {
            configs.commands.push_back(it->as_string()->get());
        }
    }
}

void is_valid_hour(const int hour)
{
    if (hour < 0 or hour > 23) {
        throw std::runtime_error("All configured hour values must be between 0 and 23");
    }
}

void is_valid_minute(const int minute)
{
    if (minute < 0 or minute > 59) {
        throw std::runtime_error("All configured minute values must be between 0 and 59");
    }
}

} // namespace

Configs read_configs_from_file()
{
    Configs configs;

    try {
        read_project_toml(configs);
    } catch (const toml::parse_error &e) {
        throw std::runtime_error(e.what());
    } catch (const std::out_of_range &e) {
        throw std::runtime_error(e.what());
    }

    is_valid_hour(configs.time_wake.tm_hour);
    is_valid_minute(configs.time_wake.tm_min);
    is_valid_hour(configs.time_cmd.tm_hour);
    is_valid_minute(configs.time_cmd.tm_min);
    is_valid_hour(configs.time_sleep.tm_hour);
    is_valid_minute(configs.time_sleep.tm_min);

    if (not utils::is_valid_suspend_state(configs.suspend_type)) {
        throw std::runtime_error("Invalid suspend type");
    }

    return configs;
}
