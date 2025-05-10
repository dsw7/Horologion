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
    configs.time_run_cmd.tm_hour = table["times"]["cmd"]["hour"].value_or<int>(8);
    configs.time_run_cmd.tm_min = table["times"]["cmd"]["minute"].value_or<int>(1);
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

bool is_unit_out_of_bounds(const int unit, const int max)
{
    if (unit < 0 or unit > max) {
        return true;
    }

    return false;
}

void is_config_file_input_sane(const Configs &configs)
{
    if (is_unit_out_of_bounds(configs.time_wake.tm_hour, 23)) {
        throw std::runtime_error("Invalid input for \"time-wake-hour\" field. Input must be between [0, 23] hours");
    }

    if (is_unit_out_of_bounds(configs.time_wake.tm_min, 59)) {
        throw std::runtime_error("Invalid input for \"time-wake-minute\" field. Input must be between [0, 59] minutes");
    }

    if (is_unit_out_of_bounds(configs.time_run_cmd.tm_hour, 23)) {
        throw std::runtime_error("Invalid input for \"time-cmd-hour\" field. Input must be between [0, 23] hours");
    }

    if (is_unit_out_of_bounds(configs.time_run_cmd.tm_min, 59)) {
        throw std::runtime_error("Invalid input for \"time-cmd-minute\" field. Input must be between [0, 59] minutes");
    }

    if (is_unit_out_of_bounds(configs.time_sleep.tm_hour, 23)) {
        throw std::runtime_error("Invalid input for \"time-sleep-hour\" field. Input must be between [0, 23] hours");
    }

    if (is_unit_out_of_bounds(configs.time_sleep.tm_min, 59)) {
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
    Configs configs;

    try {
        read_project_toml(configs);
    } catch (const toml::parse_error &e) {
        throw std::runtime_error(e.what());
    } catch (const std::out_of_range &e) {
        throw std::runtime_error(e.what());
    }

    is_config_file_input_sane(configs);
    return configs;
}
