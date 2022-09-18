#include "command_trigger.h"

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

    if (this->configs.commands.size() == 0)
    {
        Logger::info("Found no commands to run. Doing nothing");
        return true;
    }

    unsigned int delta_t = this->time_sleep - this->time_alarm;

    Logger::info("System will stay awake for " + std::to_string(delta_t) + " seconds for all subprocesses to complete");

    // replace sleep with std::this_thread::sleep_for as sleep(3) is not thread safe
    // see https://cplusplus.com/reference/thread/this_thread/sleep_for/
    sleep(delta_t);

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
