#include "command_trigger.h"

bool CommandTrigger::check_valid_suspend_state()
{
    return true;
}

bool CommandTrigger::run_commands()
{
    unsigned int delta_t = this->time_sleep - this->time_alarm;

    Logger::info("System will stay awake for " + std::to_string(delta_t) + " seconds for all subprocesses to complete");

    // replace sleep with std::this_thread::sleep_for as sleep(3) is not thread safe
    // see https://cplusplus.com/reference/thread/this_thread/sleep_for/
    sleep(delta_t);

    return true;
}

bool CommandTrigger::shutdown()
{
    Logger::info("Committing buffer cache to disk");
    sync();

    Logger::info("Shutting down system");

    if (reboot(LINUX_REBOOT_CMD_POWER_OFF) != 0)
    {
        Logger::error("Failed to shut down machine");
        Logger::error(strerror(errno));
        return false;
    }

    return true;
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

    if (not this->shutdown())
    {
        return false;
    }

    return true;
};
