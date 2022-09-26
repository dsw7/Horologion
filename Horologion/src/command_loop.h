#ifndef COMMAND_RUN_LOOP_H
#define COMMAND_RUN_LOOP_H

#include <string>
#include <ctime>
#include <chrono>
#include <csignal>
#include <thread>
#include <unistd.h>

#include "command_base.h"
#include "utils_time.h"
#include "utils_sysfs.h"

const unsigned int SECONDS_PER_DAY = 86400;

void interrupt_signal_handler(int signum);
void worker_stay_awake(std::time_t *duration, std::string *suspend_type);
void worker_run_command(std::string *target, std::string *command);

class CommandLoop: public CommandBase
{
    private:
        std::time_t time_wake = 0;
        std::time_t time_run_cmd = 0;
        std::time_t wake_duration = 0;

        bool set_times();
        void set_alarm();
        void deploy_jobs();
        void run_loop();

    public:
        void main();
};

#endif
