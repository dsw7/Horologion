#ifndef COMMAND_RUN_LOOP_H
#define COMMAND_RUN_LOOP_H

#include <string>
#include <ctime>
#include <unistd.h>

#include "command_base.h"

class CommandLoop: public CommandBase
{
    private:
        void run_loop();

    public:
        bool main();
};

#endif
