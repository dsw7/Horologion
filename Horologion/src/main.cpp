#include <string>
#include <ctime>

#include <errno.h>
#include <string.h>
#include <linux/reboot.h>  // For LINUX_REBOOT_CMD_POWER_OFF

// System calls
#include <unistd.h>        // For sync(2)
#include <sys/reboot.h>    // For reboot(2)

#include "logger.h"
#include "file_utils.h"

bool is_running_as_root()
{
    if (getuid() != 0)
    {
        Logger::error("Not running as root. Additional privileges needed!");
        return false;
    }

    return true;
}

std::time_t compute_delay(std::time_t offset_seconds)
{
    std::time_t time_since_epoch = std::time(nullptr);

    Logger::info("The current time is: ");
    Logger::info(std::asctime(std::localtime(&time_since_epoch)));

    std::time_t offset_time = time_since_epoch + offset_seconds;

    Logger::info("The offset time was computed as: ");
    Logger::info(std::asctime(std::localtime(&offset_time)));

    return offset_time;
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

int main()
{
    if (not is_running_as_root())
    {
        return EXIT_FAILURE;
    }

    if (not write_to_rtc_alarm(compute_delay(60)))
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
