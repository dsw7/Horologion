#include "utils.hpp"

#include "logger.hpp"

#include <filesystem>
#include <fstream>
#include <map>
#include <stdexcept>
#include <unistd.h>

namespace utils {

void is_running_as_root()
{
    if (getuid() == 0) {
        return;
    }

    throw std::runtime_error("Not running as root. Additional privileges needed!");
}

void write_to_file(const std::string &filepath, const std::string &contents)
{
    if (not std::filesystem::exists(filepath)) {
        throw std::runtime_error("File \"" + filepath + "\" does not exist!");
    }

    std::ofstream file;
    file.open(filepath);
    file << contents;
    file.close();
}

std::string read_from_file(const std::string &filepath)
{
    if (not std::filesystem::exists(filepath)) {
        throw std::runtime_error("File \"" + filepath + "\" does not exist!");
    }

    std::ifstream filestream(filepath);
    std::string line;
    std::string contents;

    while (getline(filestream, line)) {
        contents += line + "\n";
    }

    filestream.close();
    return contents;
}

std::string epoch_time_to_ascii_time(const std::time_t &epoch_time)
{
    std::string ascii_time = std::string(std::asctime(std::localtime(&epoch_time)));

    if (not ascii_time.empty() and ascii_time[ascii_time.length() - 1] == '\n') {
        ascii_time.erase(ascii_time.length() - 1);
    }

    return ascii_time;
}

std::time_t get_epoch_time_from_configs(const int &hour, const int &minute, const int &second)
{
    std::time_t now = std::time(nullptr);
    std::tm *tm_time = std::localtime(&now);

    tm_time->tm_hour = hour;
    tm_time->tm_min = minute;
    tm_time->tm_sec = second;

    return mktime(tm_time);
}

} // namespace utils
