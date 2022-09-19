#include "command_set_alarm.h"

bool CommandSetAlarm::main()
{
    if (not this->is_running_as_root())
    {
        return false;
    }

    if (not this->read_configs_from_file())
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

    return true;
};
