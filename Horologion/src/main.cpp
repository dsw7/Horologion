#include <string>

#include "command_line_interface.h"
#include "command_reset_alarm.h"
#include "command_set_alarm.h"
#include "command_trigger.h"

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
    else if (command.compare("reset-alarm") == 0)
    {
        CommandResetAlarm command;

        if (not command.main())
        {
            return EXIT_FAILURE;
        }
    }
    else if (command.compare("set-alarm") == 0)
    {
        CommandSetAlarm command;

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
