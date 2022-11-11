#include <string>
#include <ctime>
#include <chrono>
#include <thread>
#include <stdio.h>

#include "utils_sysfs.h"
#include "workers.h"
#include "logger.h"

void worker_stay_awake(const std::time_t *duration, std::string *suspend_type)
{
    Logger::info_thread_safe("<target_0> Keeping system awake for " + std::to_string(*duration) + " seconds");
    std::this_thread::sleep_for(std::chrono::seconds(*duration));

    Logger::info_thread_safe("<target_0> " + std::to_string(*duration) + " seconds have elapsed");

    if (suspend_type->compare("none") == 0)
    {
        Logger::info_thread_safe("<target_0> Suspend disabled. Doing nothing");
        return;
    }

    Logger::info_thread_safe("<target_0> Suspending system");
    ::suspend_system(*suspend_type);

    Logger::info_thread_safe("<target_0> Waking system and terminating this thread");
}

void worker_run_command(const std::string *target, std::string *command)
{
    std::array<char, 128> buffer;
    std::string subproc_output;

    if (command->find_first_not_of(' ') == std::string::npos)
    {
        Logger::error_thread_safe("<" + *target + "> Found empty command. Doing nothing");
        return;
    }

    if (command->find(" 2>&1") == std::string::npos)
    {
        // stderr will otherwise leak out to terminal
        *command += " 2>&1";
    }

    Logger::info_thread_safe("<" + *target + "> Deploying target. Command: \"" + *command + "\"");

    FILE* pipe = ::popen(command->c_str(), "r");

    if (!pipe)
    {
        Logger::error_thread_safe("<" + *target + "> Target could not be started");
        return;
    }

    while (::fgets(buffer.data(), 128, pipe) != NULL)
    {
        Logger::info_thread_safe("<" + *target + "> Reading output from target");
        subproc_output += buffer.data();
    }

    if (::pclose(pipe) != 0)
    {
        Logger::warning_thread_safe("<" + *target + "> Target exited with non-zero exit code");
    }

    if (subproc_output.size() > 0)
    {
        Logger::info_thread_safe("<" + *target + "> Output from target:\n" + subproc_output);
    }
    else
    {
        Logger::info_thread_safe("<" + *target + "> No output from target");
    }
}
