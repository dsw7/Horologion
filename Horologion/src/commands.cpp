#include "commands.h"

bool CommandBase::is_running_as_root()
{
    if (getuid() != 0)
    {
        Logger::error("Not running as root. Additional privileges needed!");
        return false;
    }

    return true;
}

bool CommandResetAlarm::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    return true;
};

bool CommandSetAlarm::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    return true;
};
