#include "command_trigger.h"

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

    if (not this->wakealarm_exists())
    {
        return false;
    }

    this->set_rtc_alarm();

    if (not this->shutdown())
    {
        return false;
    }

    return true;
};
