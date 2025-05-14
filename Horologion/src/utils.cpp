#include "utils.hpp"

#include "logger.hpp"

#include <filesystem>
#include <fmt/format.h>
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
        throw std::runtime_error(fmt::format("File '{}' does not exist!", filepath));
    }

    std::ofstream file;
    file.open(filepath);
    file << contents;
    file.close();
}

std::string read_from_file(const std::string &filepath)
{
    if (not std::filesystem::exists(filepath)) {
        throw std::runtime_error(fmt::format("File '{}' does not exist!", filepath));
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

} // namespace utils
