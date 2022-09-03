#include <iostream>
#include <errno.h>
#include <string.h>
#include <linux/reboot.h>  // For LINUX_REBOOT_CMD_POWER_OFF

// System calls
#include <unistd.h>        // For sync(2)
#include <sys/reboot.h>    // For reboot(2)

bool shutdown()
{
    sync();

    if (reboot(LINUX_REBOOT_CMD_POWER_OFF) != 0)
    {
        std::cerr << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

int main()
{
    if (not shutdown())
    {
        return EXIT_FAILURE;
    }

    return 0;
}
