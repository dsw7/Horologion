#include <iostream>
#include <string>
#include <ctime>

#include <errno.h>
#include <string.h>
#include <time.h>
#include <linux/reboot.h>  // For LINUX_REBOOT_CMD_POWER_OFF

// System calls
#include <unistd.h>        // For sync(2)
#include <sys/reboot.h>    // For reboot(2)

std::string get_current_datetime_string()
{
    time_t time_now = time(0);
    struct tm time_struct;
    char   time_buffer[80];

    time_struct = *localtime(&time_now);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%X", &time_struct);

    return time_buffer;
}

namespace Logger {

    void info(const std::string &message)
    {
        std::cout << get_current_datetime_string() + " I " + message << std::endl;
    }

    void warning(const std::string &message)
    {
        std::cout << get_current_datetime_string() + " W " + message << std::endl;
    }

    void error(const std::string &message)
    {
        std::cerr << get_current_datetime_string() + " E " + message << std::endl;
    }

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

bool is_running_as_root()
{
    if (getuid() != 0)
    {
        Logger::error("Not running as root. Additional privileges needed!");
        return false;
    }

    return true;
}

bool shutdown()
{
    sync();

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

    compute_delay(60);

    // temporary
    return EXIT_SUCCESS;

    if (not shutdown())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
