#include "command_trigger.h"

// ----------------------------------------------------------------------------------------------------------

void worker_stay_awake(unsigned int *wake_time)
{
    Logger::info_thread_safe("<target_0> Keeping system awake for " + std::to_string(*wake_time) + " seconds");
    std::this_thread::sleep_for(std::chrono::seconds(*wake_time));

    Logger::info_thread_safe("<target_0> " + std::to_string(*wake_time) + " seconds have elapsed");
}

void worker_run_command(std::string *target, std::string *command, int *exit_code)
{
    std::array<char, 128> buffer;
    std::string subproc_output;

    Logger::info_thread_safe("<" + *target + "> Deploying target. Command: \"" + *command + "\"");

    FILE* pipe = popen(command->c_str(), "r");

    if (!pipe)
    {
        Logger::error_thread_safe("<" + *target + "> Target could not be started");
        *exit_code = 1;
        return;
    }

    while (fgets(buffer.data(), 128, pipe) != NULL)
    {
        Logger::info_thread_safe("<" + *target + "> Reading output from target");
        subproc_output += buffer.data();
    }

    *exit_code = pclose(pipe);

    if (subproc_output.size() > 0)
    {
        Logger::info_thread_safe("<" + *target + "> Output from target:\n" + subproc_output);
    }
    else
    {
        Logger::info_thread_safe("<" + *target + "> No output from target");
    }
}

// ----------------------------------------------------------------------------------------------------------

bool CommandTrigger::sysfs_sleep_state_files_exist()
{
    Logger::info("Checking if sysfs state file exists: " + SYSFS_STATE);

    if (not file_exists(SYSFS_STATE))
    {
        return false;
    }

    return true;
}

bool CommandTrigger::check_valid_suspend_state()
{
    if (this->configs.suspend_type.compare("none") == 0)
    {
        return true;
    }

    std::string sysfs_states, state;
    read_file(SYSFS_STATE, sysfs_states);

    std::stringstream ss_states(sysfs_states);

    bool state_found = false;

    while (ss_states >> state)
    {
        if (this->configs.suspend_type.compare(state) == 0)
        {
            state_found = true;
        }
    }

    if (not state_found)
    {
        Logger::error("State \"" + this->configs.suspend_type + "\" not supported!");
        Logger::error("Valid states are: " + sysfs_states);
    }

    return state_found;
}

bool CommandTrigger::run_commands()
{
    unsigned int num_commands = this->configs.commands.size();

    if (num_commands == 0)
    {
        Logger::info("Found no commands to run. Doing nothing");
        return true;
    }

    std::vector<std::thread> jobs;

    unsigned int wake_time = this->time_sleep - this->time_alarm;
    jobs.push_back(std::thread(worker_stay_awake, &wake_time));

    std::vector<int> exit_codes(num_commands);

    for (unsigned int i = 0; i < num_commands; ++i)
    {
        jobs.push_back(
            std::thread(
                worker_run_command,
                &this->configs.commands.at(i).first,
                &this->configs.commands.at(i).second,
                &exit_codes.at(i)
            )
        );
    }

    for (unsigned int i = 0; i < jobs.size(); ++i)
    {
        jobs.at(i).join();
    }

    return true;
}

void CommandTrigger::suspend_system()
{
    if (this->configs.suspend_type.compare("none") == 0)
    {
        Logger::info("ACPI signal transmission disabled. Doing nothing");
        return;
    }

    // see https://www.kernel.org/doc/html/v4.18/admin-guide/pm/sleep-states.html disk / shutdown section
    Logger::info("Sending ACPI suspend signal to machine");

    write_to_file(SYSFS_STATE, this->configs.suspend_type);
}

bool CommandTrigger::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    if (not this->config_exists())
    {
        return false;
    }

    this->read_configs_from_file();

    if (not this->sysfs_sleep_state_files_exist())
    {
        return false;
    }

    if (not this->check_valid_suspend_state())
    {
        return false;
    }

    if (not this->wakealarm_exists())
    {
        return false;
    }

    this->set_time_alarm();
    this->set_time_sleep();

    if (not this->sanitize_wake_sleep_cycle())
    {
        return false;
    }

    this->set_rtc_alarm();

    if (not this->run_commands())
    {
        return false;
    }

    this->suspend_system();
    return true;
};
