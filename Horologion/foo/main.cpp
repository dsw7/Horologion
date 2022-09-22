#include <iostream>
#include <string>

void help_commands(char *filename)
{
    std::cerr << "\e[1m\e[4mUsage:\e[0m\n\n";
    std::cerr << "  $ " << filename << " <command> [<options>]\n\n";

    std::string commands =
    "\e[1m\e[4mCommands:\e[0m\n\n"
    "  -h, --help    Print help information and exit.\n"
    "  loop          \e[1mStart the wake-sleep cycle.\e[0m This command should be run by\n"
    "                systemd or some equivalent init system.";

    std::cerr << commands << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        help_commands(argv[0]);
        return EXIT_FAILURE;
    }

    std::string command = std::string(argv[1]);

    if (command.compare("-h") == 0 or command.compare("--help") == 0)
    {
        help_commands(argv[0]);
        return EXIT_SUCCESS;
    }
    else if (command.compare("loop") == 0)
    {
        std::cout << "loop" << std::endl;
    }
    else
    {
        help_commands(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
