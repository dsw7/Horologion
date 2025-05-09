#include "command_plan.h"

#include "utils_time.h"

#include <ctime>
#include <iostream>

void CommandPlan::set_times()
{
    std::time_t time_wake = ::get_epoch_time_from_configs(
        this->configs.time_wake.tm_hour,
        this->configs.time_wake.tm_min,
        this->configs.time_wake.tm_sec);

    std::time_t time_run_cmd = ::get_epoch_time_from_configs(
        this->configs.time_run_cmd.tm_hour,
        this->configs.time_run_cmd.tm_min,
        this->configs.time_run_cmd.tm_sec);

    std::time_t time_sleep = ::get_epoch_time_from_configs(
        this->configs.time_sleep.tm_hour,
        this->configs.time_sleep.tm_min,
        this->configs.time_sleep.tm_sec);

    this->time_wake = ::epoch_time_to_ascii_time(time_wake);
    this->time_run_cmd = ::epoch_time_to_ascii_time(time_run_cmd);
    this->time_sleep = ::epoch_time_to_ascii_time(time_sleep);
}

void CommandPlan::print_tree()
{
    std::cout << "\n[Plan]:\n\n";
    std::cout << " " + ::TL_ELBOW + ::H_LINE + "{t_w}: " + this->time_wake + "\n";
    std::cout << " " + ::V_LINE + "\n";
    std::cout << " " + ::V_LINE + "\n";
    std::cout << " " + ::RIGHT_TEE + ::H_LINE + "{t_c}: " + this->time_run_cmd + " -> [Commands]\n";
    std::cout << " " + ::V_LINE + "\n";
    std::cout << " " + ::V_LINE + "\n";
    std::cout << " " + ::BL_ELBOW + ::H_LINE + "{t_s}: " + this->time_sleep + "\n";
    std::cout << std::endl;

    unsigned int n = this->configs.commands.size();

    if (n == 0) {
        std::cout << "[Commands]: None" << std::endl;
        return;
    }

    if (n == 1) {
        std::cout << "[Commands]: " + ::H_LINE + ::H_LINE + ::H_LINE + " " + this->configs.commands[0].second << std::endl;
        return;
    }

    for (unsigned int i = 0; i < n; ++i) {
        if (i == 0) {
            std::cout << "[Commands]: " + ::H_LINE + ::DOWN_TEE + ::H_LINE + " " + this->configs.commands[i].second + "\n";
        } else if (i == n - 1) {
            std::cout << "             " + ::BL_ELBOW + ::H_LINE + " " + this->configs.commands[i].second + "\n";
        } else {
            std::cout << "             " + ::RIGHT_TEE + ::H_LINE + " " + this->configs.commands[i].second + "\n";
        }
    }

    std::cout << std::endl;
}

void CommandPlan::main()
{
    if (not this->read_configs_from_file()) {
        ::exit(EXIT_FAILURE);
    }

    if (not this->is_config_file_input_sane()) {
        ::exit(EXIT_FAILURE);
    }

    this->set_times();
    this->print_tree();
}
