#include "command_trigger.h"

bool CommandTrigger::run_commands()
{
    unsigned int delta_t = this->time_sleep - this->time_alarm;
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
