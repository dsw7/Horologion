#include "workers.hpp"

#include "logger.hpp"
#include "utils.hpp"

#include <array>
#include <chrono>
#include <stdio.h>
#include <thread>

void worker_stay_awake(const std::time_t *duration, std::string *suspend_type)
{
    logger::info_thread_safe("<target_0> Keeping system awake for " + std::to_string(*duration) + " seconds");
    std::this_thread::sleep_for(std::chrono::seconds(*duration));

    logger::info_thread_safe("<target_0> " + std::to_string(*duration) + " seconds have elapsed");

    if (suspend_type->compare("none") == 0) {
        logger::info_thread_safe("<target_0> Suspend disabled. Doing nothing");
        return;
    }

    logger::info_thread_safe("<target_0> Suspending system");
    utils::suspend_system(*suspend_type);

    logger::info_thread_safe("<target_0> Waking system and terminating this thread");
}

void worker_run_command(const std::string *target, std::string *command)
{
    if (command->find_first_not_of(' ') == std::string::npos) {
        logger::error_thread_safe("<" + *target + "> Found empty command. Doing nothing");
        return;
    }

    if (command->find(" 2>&1") == std::string::npos) {
        // stderr will otherwise leak out to terminal
        *command += " 2>&1";
    }

    logger::info_thread_safe("<" + *target + "> Deploying target. Command: \"" + *command + "\"");

    FILE *pipe = popen(command->c_str(), "r");

    if (!pipe) {
        logger::error_thread_safe("<" + *target + "> Target could not be started");
        return;
    }

    std::array<char, 128> buffer;
    std::string subproc_output;

    while (fgets(buffer.data(), 128, pipe) != NULL) {
        logger::info_thread_safe("<" + *target + "> Reading output from target");
        subproc_output += buffer.data();
    }

    if (pclose(pipe) != 0) {
        logger::warning_thread_safe("<" + *target + "> Target exited with non-zero exit code");
    }

    if (subproc_output.size() > 0) {
        logger::info_thread_safe("<" + *target + "> Output from target:\n" + subproc_output);
    } else {
        logger::info_thread_safe("<" + *target + "> No output from target");
    }
}
