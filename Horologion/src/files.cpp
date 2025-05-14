#include "files.hpp"

#include <filesystem>
#include <fmt/format.h>
#include <stdexcept>
#include <stdlib.h>

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

} // namespace

namespace files {

std::string get_project_config()
{
    std::string config = get_proj_data_dir() / "horolog.toml";

    if (not std::filesystem::exists(config)) {
        throw std::runtime_error(fmt::format("Configuration file '{}' does not exist", config));
    }

    return config;
}

std::string get_project_log()
{
    return get_proj_data_dir() / "horolog.log";
}

} // namespace files
