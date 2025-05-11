#include "command_plan.hpp"

#include "configs.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

const std::string V_LINE = "\u2502";
const std::string H_LINE = "\u2500";
const std::string RIGHT_TEE = "\u251c";
const std::string DOWN_TEE = "\u252c";
const std::string BL_ELBOW = "\u2514";
const std::string TL_ELBOW = "\u250c";

void print_schedule(const Configs &configs)
{
    std::string time_wake = utils::epoch_time_to_ascii_time(configs.time_wake_e);
    std::string time_cmd = utils::epoch_time_to_ascii_time(configs.time_cmd_e);
    std::string time_sleep = utils::epoch_time_to_ascii_time(configs.time_sleep_e);

    std::cout << "\n[Plan]:\n\n";
    std::cout << " " + TL_ELBOW + H_LINE + "{t_w}: " + time_wake + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + RIGHT_TEE + H_LINE + "{t_c}: " + time_cmd + " -> [Commands]\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + V_LINE + "\n";
    std::cout << " " + BL_ELBOW + H_LINE + "{t_s}: " + time_sleep + "\n";
}

void print_pstree(const Configs &configs)
{
    if (configs.commands.empty()) {
        std::cout << "[Commands]: None\n";
        return;
    }

    int n = configs.commands.size();

    if (n == 1) {
        std::cout << "[Commands]: " + H_LINE + H_LINE + H_LINE + " " + configs.commands[0] << '\n';
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (i == 0) {
            std::cout << "[Commands]: " + H_LINE + DOWN_TEE + H_LINE + " " + configs.commands[i] + "\n";
        } else if (i == n - 1) {
            std::cout << "             " + BL_ELBOW + H_LINE + " " + configs.commands[i] + "\n";
        } else {
            std::cout << "             " + RIGHT_TEE + H_LINE + " " + configs.commands[i] + "\n";
        }
    }
}

} // namespace

namespace commands {

void plan()
{
    Configs configs = read_configs_from_file();

    print_schedule(configs);
    std::cout << '\n';

    print_pstree(configs);
    std::cout << '\n';
}

} // namespace commands
