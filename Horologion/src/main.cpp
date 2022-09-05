#include <string>

#include <errno.h>
#include <string.h>
#include <linux/reboot.h>  // For LINUX_REBOOT_CMD_POWER_OFF

// System calls
#include <unistd.h>        // For sync(2)
#include <sys/reboot.h>    // For reboot(2)

#include "command_line_interface.h"
#include "logger.h"
#include "utils_file.h"
#include "time_utils.h"
#include "utils_alarm.h"

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

    if (not is_running_as_root())
    {
        return EXIT_FAILURE;
    }

    int when_to_wake_up = 60; // i.e. wake up in 60 seconds

    if (not set_rtc_alarm(compute_delay(when_to_wake_up)))
    {
        return EXIT_FAILURE;
    }

    bool is_dry_run = false;

    if (not shutdown(is_dry_run))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
