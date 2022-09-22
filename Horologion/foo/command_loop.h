#ifndef COMMAND_RUN_LOOP_H
#define COMMAND_RUN_LOOP_H

#include <string>
#include <ctime>
#include <csignal>
#include <unistd.h>

#include "command_base.h"
#include "utils_time.h"
#include "utils_sysfs.h"

const unsigned int SECONDS_PER_DAY = 86400;

void interrupt_signal_handler(int signum);

class CommandLoop: public CommandBase
{
    private:
        void compute_wake_sleep_window();
        void update_window_limits();
        void shift_window_by_one_day();
        void log_window_limits();
        void run_loop();

    public:
        void main();
};

#endif
