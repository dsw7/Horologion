#ifndef COMMAND_RESET_ALARM_H
#define COMMAND_RESET_ALARM_H

#include "command_base.h"

class CommandDisable: public CommandBase
{
    private:
        void unset_cron_job();

    public:
        bool main();
};

#endif
