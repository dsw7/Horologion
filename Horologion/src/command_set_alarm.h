#ifndef COMMAND_SET_ALARM_H
#define COMMAND_SET_ALARM_H

#include <string>

#include "command_base.h"

const std::string PATH_TO_BINARY = "/usr/bin/horolog";
const std::string PATH_TO_LOG = "/var/log/horolog.log";

class CommandEnable: public CommandBase
{
    private:
        void set_cron_job();

    public:
        bool main();
};

#endif
