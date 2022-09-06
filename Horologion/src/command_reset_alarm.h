#ifndef COMMAND_RESET_ALARM_H
#define COMMAND_RESET_ALARM_H

#include "command_base.h"

class CommandResetAlarm: public CommandBase
{
    public:
        bool main();
};

#endif
