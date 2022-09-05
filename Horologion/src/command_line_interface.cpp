#include "command_line_interface.h"

void help_commands(char *filename)
{
    std::cerr << "\e[1m\e[4mUsage:\e[0m:\n\n";
    std::cerr << "  $ " << filename << " <command> [<options>]\n\n";

    std::string commands =
    "\e[1m\e[4mCommands\e[0m:\n\n"
    "  -h, --help    Print help information and exit.\n"
    "  set-alarm     \e[1mSet alarm clock.\e[0m This command will write out an Epoch time to the system's"
    " wakealarm file.\n"
    "  reset-alarm   \e[1mReset alarm clock.\e[0m This command will erase the contents of the system's"
    " wakealarm file.\n"
    "  trigger       \e[1mTrigger a wake-sleep cycle.\e[0m This command should be invoked by systemd or an"
    " equivalent init system.\n";

    std::cerr << commands << std::endl;
}
