#include "command_plan.hpp"

#include "parse_config_file.hpp"
#include "utils.hpp"

#include <ctime>
#include <iostream>

namespace {
const std::string V_LINE = "\u2502";
const std::string H_LINE = "\u2500";
const std::string RIGHT_TEE = "\u251c";
const std::string DOWN_TEE = "\u252c";
const std::string BL_ELBOW = "\u2514";
const std::string TL_ELBOW = "\u250c";
} // namespace

void CommandPlan::set_times()
{
    const std::time_t time_wake_t = utils::get_epoch_time_from_configs(
        this->configs.time_wake.tm_hour,
        this->configs.time_wake.tm_min,
        this->configs.time_wake.tm_sec);

    const std::time_t time_run_cmd_t = utils::get_epoch_time_from_configs(
        this->configs.time_run_cmd.tm_hour,
        this->configs.time_run_cmd.tm_min,
        this->configs.time_run_cmd.tm_sec);

    const std::time_t time_sleep_t = utils::get_epoch_time_from_configs(
        this->configs.time_sleep.tm_hour,
        this->configs.time_sleep.tm_min,
        this->configs.time_sleep.tm_sec);

    this->time_wake = utils::epoch_time_to_ascii_time(time_wake_t);
    this->time_run_cmd = utils::epoch_time_to_ascii_time(time_run_cmd_t);
    this->time_sleep = utils::epoch_time_to_ascii_time(time_sleep_t);
}

void CommandPlan::print_tree()
{
    std::cout << "\n[Plan]:\n\n";
    std::cout << " " + TL_ELBOW + H_LINE + "{t_w}: " + this->time_wake + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + RIGHT_TEE + H_LINE + "{t_c}: " + this->time_run_cmd + " -> [Commands]\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + BL_ELBOW + H_LINE + "{t_s}: " + this->time_sleep + "\n";
    std::cout << '\n';

    unsigned int n = this->configs.commands.size();

    if (n == 0) {
        std::cout << "[Commands]: None\n";
        return;
    }

    if (n == 1) {
        std::cout << "[Commands]: " + H_LINE + H_LINE + H_LINE + " " + this->configs.commands[0].second << '\n';
        return;
    }

    for (unsigned int i = 0; i < n; ++i) {
        if (i == 0) {
            std::cout << "[Commands]: " + H_LINE + DOWN_TEE + H_LINE + " " + this->configs.commands[i].second + "\n";
        } else if (i == n - 1) {
            std::cout << "             " + BL_ELBOW + H_LINE + " " + this->configs.commands[i].second + "\n";
        } else {
            std::cout << "             " + RIGHT_TEE + H_LINE + " " + this->configs.commands[i].second + "\n";
        }
    }

    std::cout << '\n';
}

void CommandPlan::main()
{
    this->configs = read_configs_from_file();
    this->set_times();
    this->print_tree();
}
