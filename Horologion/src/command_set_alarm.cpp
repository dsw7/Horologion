#include "command_set_alarm.h"

bool CommandSetAlarm::main()
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

    return true;
};
