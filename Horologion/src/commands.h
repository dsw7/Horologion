#ifndef COMMANDS_H
#define COMMANDS_H

#include <unistd.h>

#include "logger.h"
#include "utils_file.h"

const std::string SYSFS_WAKEALARM = "/sys/class/rtc/rtc0/wakealarm";

class CommandBase
{
    protected:
        bool is_running_as_root();
        bool wakealarm_exists();
        void reset_rtc_alarm();

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
