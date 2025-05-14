#include "command_plan.hpp"

#include "configs.hpp"
#include "utils.hpp"
#include <fmt/core.h>

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

    fmt::print("\n[Plan]:\n\n");
    fmt::print(" {}{}{{t_w}}: {}\n", TL_ELBOW, H_LINE, time_wake);
    fmt::print(" {}\n", V_LINE);
    fmt::print(" {}\n", V_LINE);
    fmt::print(" {}{}{{t_c}}: {} -> [Commands]\n", RIGHT_TEE, H_LINE, time_cmd);
    fmt::print(" {}\n", V_LINE);
    fmt::print(" {}\n", V_LINE);
    fmt::print(" {}{}{{t_s}}: {}\n", BL_ELBOW, H_LINE, time_sleep);
}

void print_pstree(const Configs &configs)
{
    if (configs.commands.empty()) {
        fmt::print("[Commands]: None\n");
        return;
    }

    int n = configs.commands.size();

    if (n == 1) {
        fmt::print("[Commands]: {}{}{} {}\n", H_LINE, H_LINE, H_LINE, configs.commands[0]);
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (i == 0) {
            fmt::print("[Commands]: {}{}{} {}\n", H_LINE, DOWN_TEE, H_LINE, configs.commands[i]);
        } else if (i == n - 1) {
            fmt::print("             {}{} {}\n", BL_ELBOW, H_LINE, configs.commands[i]);
        } else {
            fmt::print("             {}{} {}\n", RIGHT_TEE, H_LINE, configs.commands[i]);
        }
    }
}

} // namespace

namespace commands {

void plan()
{
    Configs configs = read_configs_from_file();

    print_schedule(configs);
    fmt::print("\n");

    print_pstree(configs);
}

} // namespace commands
