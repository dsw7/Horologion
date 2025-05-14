#include "command_loop.hpp"
#include "command_plan.hpp"
#include "command_test.hpp"
#include "logger.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

void help_commands(const std::string &prog)
{
    std::cerr << "\033[1m\033[4mUsage:\033[0m\n\n";
    std::cerr << "  $ " << prog << " <command> [<options>]\n\n";

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

void run_command(const std::string &prog, const std::string &first_arg)
{
    if (first_arg == "-h" or first_arg == "--help") {
        help_commands(prog);
    } else if (first_arg == "loop") {
        commands::loop();
    } else if (first_arg == "plan") {
        commands::plan();
    } else if (first_arg == "test") {
        commands::test();
    } else {
        help_commands(prog);
    }
}

int main(int argc, char **argv)
{
    const std::string prog = std::string(argv[0]);

    if (argc < 2) {
        help_commands(prog);
        return EXIT_FAILURE;
    }

    try {
        run_command(prog, std::string(argv[1]));
    } catch (const std::runtime_error &e) {
        logger::error(e.what());
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
