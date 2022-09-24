#ifndef COMMAND_PLAN_H
#define COMMAND_PLAN_H

#include <iostream>
#include <string>
#include <ctime>

#include "utils_time.h"
#include "command_base.h"

const std::string V_LINE = "\u2502";
const std::string H_LINE = "\u2500";
const std::string RIGHT_TEE = "\u251c";
const std::string DOWN_TEE = "\u252c";
const std::string BL_ELBOW = "\u2514";
const std::string TL_ELBOW = "\u250c";

class CommandPlan: public CommandBase
{
    private:
        std::string time_wake = "";
        std::string time_run_cmd = "";
        std::string time_sleep = "";

        void set_times();
        void print_tree();

    public:
        void main();
};

#endif
