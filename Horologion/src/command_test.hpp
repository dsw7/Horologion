#pragma once

#include "command_base.h"

class CommandTest: public CommandBase {
public:
    void main();

private:
    void deploy_jobs();
};
