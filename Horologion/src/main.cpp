#include <iostream>
#include <string>

#include <errno.h>
#include <string.h>
#include <linux/reboot.h>  // For LINUX_REBOOT_CMD_POWER_OFF

// System calls
#include <unistd.h>        // For sync(2)
#include <sys/reboot.h>    // For reboot(2)

#include "logger.h"
#include "file_utils.h"
#include "time_utils.h"

bool is_running_as_root()
{
    if (getuid() != 0)
    {
        Logger::error("Not running as root. Additional privileges needed!");
        return false;
    }

    return true;
}

bool write_to_rtc_alarm(std::time_t wakeup_time)
{
    static std::string wakealarm = "/sys/class/rtc/rtc0/wakealarm";

    Logger::info("Attempting to modify file " + wakealarm);

    if (not file_exists(wakealarm))
    {
        return false;
    }

    Logger::info("Resetting alarm");

    std::string reset_str = "0";
    write_to_file(wakealarm, reset_str);

    Logger::info("Now setting actual alarm");

    std::string str_wakeup_time = std::to_string(wakeup_time);
    write_to_file(wakealarm, str_wakeup_time);

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

void help_commands(char *filename)
{
    std::cerr << "\e[1m\e[4mUsage:\e[0m:\n\n";
    std::cerr << "  $ " << filename << " <command> [<options>]\n\n";

    std::string commands =
    "\e[1m\e[4mCommands\e[0m:\n\n"
    "  -h, --help  Print help information and exit.\n"
    "  set-alarm   \e[1mSet alarm clock.\e[0m This command will write out an Epoch time to the system's"
    " wakealarm file.\n"
    "  trigger     \e[1mTrigger a wake-sleep cycle.\e[0m This command should be invoked by systemd or an"
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

    std::cout << argv[0] << std::endl;
    argv++;
    std::cout << argv[0] << std::endl;

    if (not is_running_as_root())
    {
        return EXIT_FAILURE;
    }

    int when_to_wake_up = 60; // i.e. wake up in 60 seconds

    if (not write_to_rtc_alarm(compute_delay(when_to_wake_up)))
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
