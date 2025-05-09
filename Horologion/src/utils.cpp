#include "utils.hpp"

#include "logger.hpp"

#include <fstream>
#include <map>
#include <sstream>
#include <sys/stat.h>

namespace {

const std::string SYSFS_WAKEALARM = "/sys/class/rtc/rtc0/wakealarm";
const std::string SYSFS_STATE = "/sys/power/state";

bool file_exists(const std::string &filepath)
{
    struct stat info;

    if (::stat(filepath.c_str(), &info) != 0) {
        return false;
    }

    return true;
}

} // namespace

namespace utils {

bool write_to_file(const std::string &filepath, const std::string &message)
{
    if (not file_exists(filepath)) {
        Logger::error("File \"" + filepath + "\" does not exist!");
        return false;
    }

    Logger::info("Will write \"" + message + "\" to " + filepath);

    std::ofstream file;

    file.open(filepath);
    file << message;
    file.close();

    return true;
}

bool read_file(const std::string &filepath, std::string &file_contents)
{
    if (not file_exists(filepath)) {
        Logger::error("File \"" + filepath + "\" does not exist!");
        return false;
    }

    Logger::info("Reading " + filepath);

    std::ifstream filestream(filepath);

    std::string line;
    while (getline(filestream, line)) // macro defined getline?
    {
        file_contents += line + "\n";
    }

    filestream.close();
    return true;
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

bool unset_rtc_alarm()
{
    Logger::info("Unsetting alarm");
    std::string unset_str = "0";

    return utils::write_to_file(::SYSFS_WAKEALARM, unset_str);
}

bool set_rtc_alarm(const unsigned int wake_time)
{
    Logger::info("Setting alarm");
    std::string str_wake_time = std::to_string(wake_time);

    return utils::write_to_file(::SYSFS_WAKEALARM, str_wake_time);
}

bool is_valid_suspend_state(const std::string &state_from_ini)
{
    Logger::info("Checking whether \"" + state_from_ini + "\" is a supported suspend state");

    std::string sysfs_states, state;

    if (not utils::read_file(::SYSFS_STATE, sysfs_states)) {
        return false;
    }

    std::stringstream ss_states(sysfs_states);

    bool is_valid_state = false;

    while (ss_states >> state) {
        if (state_from_ini.compare(state) == 0) {
            is_valid_state = true;
        }
    }

    if (not is_valid_state) {
        Logger::error("State \"" + state_from_ini + "\" not supported!");
        Logger::error("Valid states are: " + sysfs_states);
    }

    return is_valid_state;
}

bool suspend_system(const std::string &suspend_type)
{
    if (suspend_type.compare("none") == 0) {
        Logger::info("ACPI signal transmission disabled. Doing nothing");
        return true;
    }

    static std::map<std::string, std::string> suspend_types = {
        { "standby", "S1 (Power-On Suspend)" },
        { "mem", "S3 (Suspend-to-RAM)" },
        { "disk", "S4 (Suspend-to-Disk)" }
    };

    // see https://www.kernel.org/doc/html/v4.18/admin-guide/pm/sleep-states.html disk / shutdown section
    Logger::info("Suspending system to state " + suspend_types[suspend_type]);

    return utils::write_to_file(::SYSFS_STATE, suspend_type);
}

} // namespace utils
