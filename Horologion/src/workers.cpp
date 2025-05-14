#include "workers.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <array>
#include <chrono>
#include <cstdio>
#include <fmt/format.h>
#include <map>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <thread>

namespace {

void suspend_system(const std::string &suspend_type)
{
    static std::map<std::string, std::string> suspend_types = {
        { "standby", "S1 (Power-On Suspend)" },
        { "mem", "S3 (Suspend-to-RAM)" },
        { "disk", "S4 (Suspend-to-Disk)" }
    };

    if (not suspend_types.count(suspend_type)) {
        throw std::runtime_error(fmt::format("Invalid suspend type: '{}'", suspend_type));
    }

    // see https://www.kernel.org/doc/html/v4.18/admin-guide/pm/sleep-states.html disk / shutdown section
    logger::info_thread_safe(fmt::format("Suspending system to state {}", suspend_types[suspend_type]));

    static std::string sysfs_state_file = "/sys/power/state";
    utils::write_to_file(sysfs_state_file, suspend_type);
}

} // namespace

namespace workers {

void stay_awake(const std::time_t duration, const std::string &suspend_type)
{
    logger::info_thread_safe(fmt::format("Keeping system awake for {} seconds", duration));
    std::this_thread::sleep_for(std::chrono::seconds(duration));

    logger::info_thread_safe(fmt::format("{} seconds have elapsed", duration));

    if (suspend_type == "none") {
        logger::info_thread_safe("ACPI signal transmission disabled. Doing nothing");
        return;
    }

    logger::info_thread_safe("Suspending system");
    suspend_system(suspend_type);

    logger::info_thread_safe("Waking system and terminating this thread");
}

void run_command(const std::string &command)
{
    std::string command_r = command;

    if (command_r.find(" 2>&1") == std::string::npos) {
        command_r += " 2>&1";
    }

    logger::info_thread_safe(fmt::format("Deploying command: '{}'", command_r));

    std::array<char, 128> buffer;
    std::string command_stdout;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command_r.c_str(), "r"), pclose);
    if (!pipe) {
        logger::error_thread_safe("Could not start command");
        return;
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        command_stdout += buffer.data();
    }

    if (command_stdout.empty()) {
        logger::info_thread_safe("No output from command");
    } else {
        logger::info_thread_safe(fmt::format("Output from command:\n{}", command_stdout));
    }
}

} // namespace workers
