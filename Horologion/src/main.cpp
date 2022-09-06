#include <string>

#include <errno.h>
#include <string.h>
#include <linux/reboot.h>  // For LINUX_REBOOT_CMD_POWER_OFF

// System calls
#include <unistd.h>        // For sync(2)
#include <sys/reboot.h>    // For reboot(2)

#include "command_line_interface.h"
#include "logger.h"
#include "utils_time.h"
#include "utils_alarm.h"
#include "commands.h"

bool is_running_as_root()
{
    if (getuid() != 0)
    {
        Logger::error("Not running as root. Additional privileges needed!");
        return false;
    }

    return true;
}

bool shutdown(bool is_dry_run)
{
    if (is_dry_run)
    {
        Logger::info("Dry run instruction provided. Will not shut down machine");
        return true;
    }

    Logger::info("Committing buffer cache to disk");
    sync();

    Logger::info("Shutting down system");

    if (reboot(LINUX_REBOOT_CMD_POWER_OFF) != 0)
    {
        Logger::error("Failed to shut down machine");
        Logger::error(strerror(errno));
        return false;
    }

    return true;
}

bool command_set_alarm()
{
    if (not is_running_as_root())
    {
        return false;
    }

    int when_to_wake_up = 60;

    return set_rtc_alarm(compute_delay(when_to_wake_up));
}

bool command_trigger()
{
    if (not is_running_as_root())
    {
        return false;
    }

    // This will be replaced with custom options that come from a config file
    int when_to_wake_up = 60; // i.e. wake up in 60 seconds

    if (not set_rtc_alarm(compute_delay(when_to_wake_up)))
    {
        return false;
    }

    bool is_dry_run = false;

    return shutdown(is_dry_run);
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
        if (not command_set_alarm())
        {
            return EXIT_FAILURE;
        }
    }
    else if (command.compare("trigger") == 0)
    {
        if (not command_trigger())
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
