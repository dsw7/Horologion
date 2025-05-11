#include "workers.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <array>
#include <chrono>
#include <map>
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
        throw std::runtime_error("Invalid suspend type: \"" + suspend_type + "\"");
    }

    // see https://www.kernel.org/doc/html/v4.18/admin-guide/pm/sleep-states.html disk / shutdown section
    logger::info_thread_safe("Suspending system to state " + suspend_types[suspend_type]);

    static std::string sysfs_state_file = "/sys/power/state";
    utils::write_to_file(sysfs_state_file, suspend_type);
}

} // namespace

namespace workers {

void worker_stay_awake(const std::time_t *duration, std::string *suspend_type)
{
    logger::info_thread_safe("<wake> Keeping system awake for " + std::to_string(*duration) + " seconds");
    std::this_thread::sleep_for(std::chrono::seconds(*duration));

    logger::info_thread_safe("<wake> " + std::to_string(*duration) + " seconds have elapsed");

    if (*suspend_type == "none") {
        logger::info_thread_safe("<wake> ACPI signal transmission disabled. Doing nothing");
        return;
    }

    logger::info_thread_safe("<wake> Suspending system");
    suspend_system(*suspend_type);

    logger::info_thread_safe("<wake> Waking system and terminating this thread");
}

void worker_run_command(std::string *command)
{
    if (command->find(" 2>&1") == std::string::npos) {
        // stderr will otherwise leak out to terminal
        *command += " 2>&1";
    }

    logger::info_thread_safe("Deploying command: \"" + *command + "\"");

    FILE *pipe = popen(command->c_str(), "r");

    if (!pipe) {
        logger::error_thread_safe("Could not start command: \"" + *command + "\"");
        return;
    }

    std::array<char, 128> buffer;
    std::string subproc_output;

    while (fgets(buffer.data(), 128, pipe) != NULL) {
        logger::info_thread_safe("Reading output from command: \"" + *command + "\"");
        subproc_output += buffer.data();
    }

    if (pclose(pipe) != 0) {
        logger::warning_thread_safe("Command \"" + *command + "\" exited with non-zero exit code");
    }

    if (subproc_output.size() > 0) {
        logger::info_thread_safe("Output from command: \"" + *command + "\" -> " + subproc_output);
    } else {
        logger::info_thread_safe("Output from command: \"" + *command + "\" -> None");
    }
}

} // namespace workers
