#include "configs.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <filesystem>
#include <iostream>
#include <sstream>
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

std::time_t config_time_to_epoch_time(const int hour, const int minute)
{
    is_valid_hour(hour);
    is_valid_minute(minute);

    std::time_t now = std::time(nullptr);
    std::tm *tm_time = std::localtime(&now);

    tm_time->tm_hour = hour;
    tm_time->tm_min = minute;
    tm_time->tm_sec = 0;

    return mktime(tm_time);
}

void read_project_toml(Configs &configs)
{
    static std::string prog_config = get_proj_data_dir() / "horolog.toml";

    if (not std::filesystem::exists(prog_config)) {
        throw std::runtime_error("Configuration file \"" + prog_config + "\" does not exist");
    }

    toml::table table = toml::parse_file(prog_config);

    configs.time_wake_e = config_time_to_epoch_time(
        table["times"]["wake"]["hour"].value_or<int>(8),
        table["times"]["wake"]["minute"].value_or<int>(0));
    configs.time_cmd_e = config_time_to_epoch_time(
        table["times"]["cmd"]["hour"].value_or<int>(8),
        table["times"]["cmd"]["minute"].value_or<int>(1));
    configs.time_sleep_e = config_time_to_epoch_time(
        table["times"]["sleep"]["hour"].value_or<int>(8),
        table["times"]["sleep"]["minute"].value_or<int>(2));

    configs.suspend_type = table["suspend-type"].value_or<std::string>("mem");

    const auto &targets = table["targets"];
    if (targets.is_array()) {
        for (auto it = targets.as_array()->begin(); it < targets.as_array()->end(); ++it) {
            configs.commands.push_back(it->as_string()->get());
        }
    }
}

void is_valid_schedule(const Configs &configs)
{
    if (configs.time_cmd_e - configs.time_wake_e < 60) {
        throw std::runtime_error("The [times.cmd] time must be at least 60 seconds ahead of [times.wake]");
    }

    if (configs.time_sleep_e - configs.time_cmd_e < 60) {
        throw std::runtime_error("The [times.sleep] time must be at least 60 seconds ahead of [times.cmd]");
    }
}

void is_valid_suspend_state(const std::string &suspend_type)
{
    logger::info("Checking whether \"" + suspend_type + "\" is a supported suspend state");

    static std::string sysfs_state_file = "/sys/power/state";
    std::string sysfs_states = utils::read_from_file(sysfs_state_file);

    bool is_valid_state = false;
    std::stringstream ss_states(sysfs_states);
    std::string state;

    while (ss_states >> state) {
        if (suspend_type == state) {
            is_valid_state = true;
        }
    }

    if (not is_valid_state) {
        throw std::runtime_error("Suspend type \"" + suspend_type + "\" not supported!\nValid states are: " + sysfs_states);
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

    is_valid_schedule(configs);
    is_valid_suspend_state(configs.suspend_type);
    return configs;
}
