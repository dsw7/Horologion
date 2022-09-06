#ifndef COMMANDS_H
#define COMMANDS_H

#include <unistd.h>
#include "logger.h"

class CommandBase
{
    protected:
        bool is_running_as_root();

    public:
        virtual bool main() {};
};

class CommandResetAlarm: public CommandBase
{
    public:
        bool main();
};

class CommandSetAlarm: public CommandBase
{
    public:
        bool main();
};

#endif
