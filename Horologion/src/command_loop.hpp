#pragma once

#include "command_base.hpp"

#include <ctime>

class CommandLoop: public CommandBase {
public:
    void main() override;

private:
    std::time_t time_wake = 0;
    std::time_t time_run_cmd = 0;
    std::time_t wake_duration = 0;

    bool set_times();
    void set_alarm();
    void deploy_jobs();
    void run_loop();
};
