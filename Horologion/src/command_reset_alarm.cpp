#include "command_reset_alarm.h"

bool CommandResetAlarm::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    return this->reset_rtc_alarm();
};
