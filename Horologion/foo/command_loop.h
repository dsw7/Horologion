#ifndef COMMAND_RUN_LOOP_H
#define COMMAND_RUN_LOOP_H

#include <string>
#include <ctime>
#include <csignal>
#include <unistd.h>

#include "command_base.h"

bool LOOP_ENABLED = true;

void interrupt_signal_handler(int signum);

class CommandLoop: public CommandBase
{
    private:
        void run_loop();

    public:
        bool main();
};

#endif
