#ifndef COMMAND_TRIGGER_H
#define COMMAND_TRIGGER_H

#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <vector>
#include <array>
#include <stdio.h>

#include "logger.h"
#include "command_base.h"

const std::string SYSFS_STATE = "/sys/power/state";

// ----------------------------------------------------------------------------------------------------------

void worker_stay_awake(unsigned int *wake_time);
void worker_run_command(std::string *target, std::string *command);

// ----------------------------------------------------------------------------------------------------------

class CommandTrigger: public CommandBase
{
    private:
        bool check_valid_suspend_state();
        void run_commands();
        bool suspend_system();

    public:
        bool main();
};

#endif
