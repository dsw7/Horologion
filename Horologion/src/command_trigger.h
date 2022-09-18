#ifndef COMMAND_TRIGGER_H
#define COMMAND_TRIGGER_H

#include <string>
#include <sstream>
#include <chrono>
#include <thread>

#include "logger.h"
#include "command_base.h"

const std::string SYSFS_STATE = "/sys/power/state";

// ----------------------------------------------------------------------------------------------------------

void worker_stay_awake(unsigned int &wake_time);

// ----------------------------------------------------------------------------------------------------------

class CommandTrigger: public CommandBase
{
    private:
        bool sysfs_sleep_state_files_exist();
        bool check_valid_suspend_state();
        bool run_commands();
        void suspend_system();

    public:
        bool main();
};

#endif
