#include <iostream>
#include <string>

#include "command_reset_alarm.h"
#include "command_set_alarm.h"
#include "command_trigger.h"

void help_commands(char *filename)
{
    std::cerr << "\e[1m\e[4mUsage:\e[0m:\n\n";
    std::cerr << "  $ " << filename << " <command> [<options>]\n\n";

    std::string commands =
    "\e[1m\e[4mCommands\e[0m:\n\n"
    "  -h, --help    Print help information and exit.\n"
    "  enable     \e[1mSet alarm clock.\e[0m This command will write out an Epoch time to the system's"
    " wakealarm file.\n"
    "  disable   \e[1mReset alarm clock.\e[0m This command will erase the contents of the system's"
    " wakealarm file.\n"
    "  trigger       \e[1mTrigger a wake-sleep cycle.\e[0m This command should be invoked by systemd or an"
    " equivalent init system.\n";

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
    else if (command.compare("disable") == 0)
    {
        CommandDisable command;

        if (not command.main())
        {
            return EXIT_FAILURE;
        }
    }
    else if (command.compare("enable") == 0)
    {
        CommandEnable command;

        if (not command.main())
        {
            return EXIT_FAILURE;
        }
    }
    else if (command.compare("trigger") == 0)
    {
        CommandTrigger command;

        if (not command.main())
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        help_commands(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
