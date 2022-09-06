#ifndef COMMAND_SET_ALARM_H
#define COMMAND_SET_ALARM_H

#include "command_base.h"

class CommandSetAlarm: public CommandBase
{
    public:
        bool main();
};

#endif
