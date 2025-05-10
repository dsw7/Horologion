#include "command_plan.hpp"

#include "configs.hpp"
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

struct Times {
    std::string time_wake;
    std::string time_run_cmd;
    std::string time_sleep;
};

Times set_times(const Configs &configs)
{
    const std::time_t time_wake_t = utils::get_epoch_time_from_configs(
        configs.time_wake.tm_hour,
        configs.time_wake.tm_min,
        configs.time_wake.tm_sec);

    const std::time_t time_run_cmd_t = utils::get_epoch_time_from_configs(
        configs.time_run_cmd.tm_hour,
        configs.time_run_cmd.tm_min,
        configs.time_run_cmd.tm_sec);

    const std::time_t time_sleep_t = utils::get_epoch_time_from_configs(
        configs.time_sleep.tm_hour,
        configs.time_sleep.tm_min,
        configs.time_sleep.tm_sec);

    Times times;
    times.time_wake = utils::epoch_time_to_ascii_time(time_wake_t);
    times.time_run_cmd = utils::epoch_time_to_ascii_time(time_run_cmd_t);
    times.time_sleep = utils::epoch_time_to_ascii_time(time_sleep_t);

    return times;
}

void print_tree(const Configs &configs, const Times &times)
{
    std::cout << "\n[Plan]:\n\n";
    std::cout << " " + TL_ELBOW + H_LINE + "{t_w}: " + times.time_wake + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + RIGHT_TEE + H_LINE + "{t_c}: " + times.time_run_cmd + " -> [Commands]\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + BL_ELBOW + H_LINE + "{t_s}: " + times.time_sleep + "\n";
    std::cout << '\n';

    unsigned int n = configs.commands.size();

    if (n == 0) {
        std::cout << "[Commands]: None\n";
        return;
    }

    if (n == 1) {
        std::cout << "[Commands]: " + H_LINE + H_LINE + H_LINE + " " + configs.commands[0].second << '\n';
        return;
    }

    for (unsigned int i = 0; i < n; ++i) {
        if (i == 0) {
            std::cout << "[Commands]: " + H_LINE + DOWN_TEE + H_LINE + " " + configs.commands[i].second + "\n";
        } else if (i == n - 1) {
            std::cout << "             " + BL_ELBOW + H_LINE + " " + configs.commands[i].second + "\n";
        } else {
            std::cout << "             " + RIGHT_TEE + H_LINE + " " + configs.commands[i].second + "\n";
        }
    }

    std::cout << '\n';
}

} // namespace

namespace commands {

void plan()
{
    Configs configs = read_configs_from_file();
    Times times = set_times(configs);
    print_tree(configs, times);
}

} // namespace commands
