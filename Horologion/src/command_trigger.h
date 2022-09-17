#ifndef COMMAND_TRIGGER_H
#define COMMAND_TRIGGER_H

#include <string>
#include <errno.h>
#include <string.h>
#include <linux/reboot.h>  // For LINUX_REBOOT_CMD_POWER_OFF

// System calls
#include <unistd.h>        // For sync(2)
#include <sys/reboot.h>    // For reboot(2)

#include "logger.h"
#include "command_base.h"

const std::string SYSFS_STATE = "/sys/power/state";

class CommandTrigger: public CommandBase
{
    private:
        bool sysfs_sleep_state_files_exist();
        bool check_valid_suspend_state();
        bool run_commands();
        bool shutdown();

    public:
        bool main();
};

#endif
