#pragma once

#include "configs.hpp"

#include <string>

class CommandPlan {
public:
    void main();

private:
    std::string time_wake;
    std::string time_run_cmd;
    std::string time_sleep;
    Configs configs;

    void set_times();
    void print_tree();
};
