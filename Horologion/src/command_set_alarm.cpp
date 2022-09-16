#include "command_set_alarm.h"

bool CommandSetAlarm::main()
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
    this->set_rtc_alarm();

    return true;
};
