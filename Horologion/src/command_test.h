#pragma once

#include <vector>
#include <thread>

#include "command_base.h"
#include "logger.h"
#include "workers.h"

class CommandTest: public CommandBase
{
    private:
        void deploy_jobs();

    public:
        void main();
};
