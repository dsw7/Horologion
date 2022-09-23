#ifndef COMMAND_RUN_LOOP_H
#define COMMAND_RUN_LOOP_H

#include <string>
#include <ctime>
#include <csignal>
#include <thread>
#include <unistd.h>

#include "command_base.h"
#include "utils_time.h"
#include "utils_sysfs.h"

const unsigned int SECONDS_PER_DAY = 86400;

// ----------------------------------------------------------------------------------------------------------

void worker_run_command(std::string *target, std::string *command);

// ----------------------------------------------------------------------------------------------------------

void interrupt_signal_handler(int signum);

class CommandLoop: public CommandBase
{
    private:
        std::time_t time_wake = 0;
        std::time_t wake_duration = 0;

        void compute_wake_sleep_window();
        bool set_wake_duration();
        void shift_window_by_one_day();
        void log_window_limits();
        void deploy_jobs();
        void run_loop();

    public:
        void main();
};

#endif
