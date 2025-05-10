#include "command_loop.hpp"
#include "command_plan.hpp"
#include "command_test.hpp"

#include <iostream>
#include <string>

void help_commands(const char *filename)
{
    std::cerr << "\033[1m\033[4mUsage:\033[0m\n\n";
    std::cerr << "  $ " << filename << " <command> [<options>]\n\n";

    const std::string commands = "\033[1m\033[4mCommands:\033[0m\n\n"
                                 "  -h, --help    Print help information and exit.\n"
                                 "  loop          \033[1mStart the wake-sleep cycle.\033[0m This command should be run by\n"
                                 "                systemd or some equivalent init system.\n"
                                 "  plan          \033[1mPrint a schematic outlining current cycle plan.\033[0m Use this command\n"
                                 "                to visually depict each cycle.\n"
                                 "  test          \033[1mRun targets specified in program configuration file.\033[0m This command should only\n"
                                 "                be used for debugging.";

    std::cerr << commands << '\n';
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        help_commands(argv[0]);
        return EXIT_FAILURE;
    }

    const std::string first_arg = std::string(argv[1]);

    if (first_arg == "-h" or first_arg == "--help") {
        help_commands(argv[0]);
        return EXIT_SUCCESS;
    } else if (first_arg == "loop") {
        CommandLoop command;
        command.main();
    } else if (first_arg == "plan") {
        CommandPlan command;
        command.main();
    } else if (first_arg == "test") {
        CommandTest command;
        command.main();
    } else {
        help_commands(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
