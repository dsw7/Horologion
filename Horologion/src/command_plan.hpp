#pragma once

#include "command_base.hpp"

#include <string>

class CommandPlan: public CommandBase {
public:
    void main();

private:
    std::string time_wake = "";
    std::string time_run_cmd = "";
    std::string time_sleep = "";

    void set_times();
    void print_tree();
};
