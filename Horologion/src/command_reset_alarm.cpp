#include "command_reset_alarm.h"

bool CommandResetAlarm::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    if (not this->wakealarm_exists())
    {
        return false;
    }

    this->reset_rtc_alarm();

    return true;
};
