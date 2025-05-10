#pragma once

#include "configs.hpp"

#include <ctime>

class CommandLoop {
public:
    void main();

private:
    std::time_t time_wake = 0;
    std::time_t time_run_cmd = 0;
    std::time_t wake_duration = 0;
    Configs configs;

    bool set_times();
    void set_alarm();
    void deploy_jobs();
    void run_loop();
};
