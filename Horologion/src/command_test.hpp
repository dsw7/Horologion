#pragma once

#include "command_base.hpp"

class CommandTest: public CommandBase {
public:
    void main();

private:
    void deploy_jobs();
};
