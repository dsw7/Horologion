#include "utils_sysfs.hpp"

#include "logger.hpp"
#include "utils_file.hpp"

#include <map>
#include <sstream>

bool unset_rtc_alarm()
{
    Logger::info("Unsetting alarm");
    std::string unset_str = "0";

    return write_to_file(::SYSFS_WAKEALARM, unset_str);
}

bool set_rtc_alarm(const unsigned int wake_time)
{
    Logger::info("Setting alarm");
    std::string str_wake_time = std::to_string(wake_time);

    return write_to_file(::SYSFS_WAKEALARM, str_wake_time);
}

bool is_valid_suspend_state(const std::string &state_from_ini)
{
    Logger::info("Checking whether \"" + state_from_ini + "\" is a supported suspend state");

    std::string sysfs_states, state;

    if (not read_file(::SYSFS_STATE, sysfs_states)) {
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

    return write_to_file(::SYSFS_STATE, suspend_type);
}
