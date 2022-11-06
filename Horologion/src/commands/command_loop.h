#pragma once

#include <ctime>

#include "command_base.h"

const unsigned int SECONDS_PER_DAY = 86400;

void interrupt_signal_handler(int signum);

class CommandLoop: public CommandBase
{
    public:
        void main();

    private:
        std::time_t time_wake = 0;
        std::time_t time_run_cmd = 0;
        std::time_t wake_duration = 0;

        bool set_times();
        void set_alarm();
        void deploy_jobs();
        void run_loop();
};
