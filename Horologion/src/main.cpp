#include "command_loop.hpp"
#include "command_plan.hpp"
#include "command_test.hpp"

#include <iostream>
#include <string>

void help_commands(const char *filename)
{
    std::cerr << "\033[1m\033[4mUsage:\033[0m\n\n";
    std::cerr << "  $ " << filename << " <command> [<options>]\n\n";

    std::string commands = "\033[1m\033[4mCommands:\033[0m\n\n"
                           "  -h, --help    Print help information and exit.\n"
                           "  loop          \033[1mStart the wake-sleep cycle.\033[0m This command should be run by\n"
                           "                systemd or some equivalent init system.\n"
                           "  plan          \033[1mPrint a schematic outlining current cycle plan.\033[0m Use this command\n"
                           "                to visually depict each cycle.\n"
                           "  test          \033[1mRun targets specified in program configuration file.\033[0m This command should only\n"
                           "                be used for debugging.";

    std::cerr << commands << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        ::help_commands(argv[0]);
        return EXIT_FAILURE;
    }

    std::string command = std::string(argv[1]);

    if (command.compare("-h") == 0 or command.compare("--help") == 0) {
        ::help_commands(argv[0]);
        return EXIT_SUCCESS;
    } else if (command.compare("loop") == 0) {
        CommandLoop command;
        command.main();
    } else if (command.compare("plan") == 0) {
        CommandPlan command;
        command.main();
    } else if (command.compare("test") == 0) {
        CommandTest command;
        command.main();
    } else {
        ::help_commands(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
